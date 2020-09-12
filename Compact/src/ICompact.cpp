#include "../include/ICompact.h"
#include "CompactImpl.cpp"
#include <cmath>    // fabs (C++11), isnan
#include <new>      // nothrow
#include <assert.h> // assert

enum VectorComparison {
    VC_LESSER       = -1,
    VC_INCOMPARABLE =  0,
    VC_BIGGER       =  1
};

// left <comparison sign> right
static VectorComparison compare(IVector const* const l, IVector const* const r, double tolerance) {
    if (l == nullptr || r == nullptr ||
        l->getDim() != r->getDim() ||
        l->getDim() == 0)
        return VectorComparison::VC_INCOMPARABLE;

    VectorComparison vc = VectorComparison::VC_INCOMPARABLE;
    for (size_t i = 0; i < l->getDim(); ++i) {
        if (fabs(l->getCoord(i) - r->getCoord(i)) < tolerance)
            continue;

        if (l->getCoord(i) > r->getCoord(i)) {
            if (vc ==  VectorComparison::VC_LESSER)
                return VectorComparison::VC_INCOMPARABLE;
            vc = VectorComparison::VC_BIGGER;
        }
        else if (l->getCoord(i) < r->getCoord(i)) {
            if (vc ==  VectorComparison::VC_BIGGER)
                return VectorComparison::VC_INCOMPARABLE;
            vc = VectorComparison::VC_LESSER;
        }
    }

    return vc;
}

static ReturnCode checkVectors(IVector const* comp1, IVector const* comp2) {
    if (comp1 == nullptr || comp2 == nullptr) {
        return ReturnCode::RC_NULL_PTR;
    }

    if (comp1->getDim() != comp2->getDim()) {
        return ReturnCode::RC_WRONG_DIM;
    }

    if (comp1->getDim() == 0) {
        return ReturnCode::RC_ZERO_DIM;
    }

    return ReturnCode::RC_SUCCESS;
}

static ReturnCode checkCompacts(ICompact const* comp1, ICompact const* comp2) {
    if (comp1 == nullptr || comp2 == nullptr) {
        return ReturnCode::RC_NULL_PTR;
    }

    if (comp1->getDim() != comp2->getDim()) {
        return ReturnCode::RC_WRONG_DIM;
    }

    if (comp1->getDim() == 0) {
        return ReturnCode::RC_ZERO_DIM;
    }

    return ReturnCode::RC_SUCCESS;
}

ICompact::Iterator::~Iterator() {}

ICompact::~ICompact() {}

ICompact* ICompact::createCompact(IVector const* begin, IVector const* end, double tolerance, ILogger* logger) {
    ReturnCode rc = checkVectors(begin, end);
    if (rc != ReturnCode::RC_SUCCESS) {
        LOG(logger, rc);
        return nullptr;
    }

    if (std::isnan(tolerance)) {
        LOG(logger, ReturnCode::RC_NAN);
        return nullptr;
    }

    if (tolerance < 0.0) {
        LOG(logger, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }

    // exclude degeneracy of compact
    for (size_t i = 0; i < begin->getDim(); ++i) {
        if (fabs(begin->getCoord(i) - end->getCoord(i)) < tolerance) {
            LOG(logger, ReturnCode::RC_INVALID_PARAMS);
            return nullptr;
        }
    }

    if (compare(begin, end, tolerance) != VectorComparison::VC_LESSER) {
        LOG(logger, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }

    ICompact* compact = new(std::nothrow) CompactImpl(begin, end, tolerance);
    if (compact == nullptr) {
        LOG(logger, ReturnCode::RC_NO_MEM);
        return nullptr;
    }

    return compact;
}

ICompact* ICompact::_union(ICompact const* comp1, ICompact const* comp2, double tolerance, ILogger* logger) {
    ReturnCode rc = checkCompacts(comp1, comp2);
    if (rc != ReturnCode::RC_SUCCESS) {
        LOG(logger, rc);
        return nullptr;
    }

    if (std::isnan(tolerance)) {
        LOG(logger, ReturnCode::RC_NAN);
        return nullptr;
    }

    if (tolerance < 0.0) {
        LOG(logger, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }

    bool subsetFlag = false;

    rc = comp2->isSubset(comp1, subsetFlag);
    if (rc == ReturnCode::RC_SUCCESS && subsetFlag) {
        ICompact* result = comp1->clone();
        if (result == nullptr) {
            LOG(logger, ReturnCode::RC_NO_MEM);
            return nullptr;
        }
        return result;
    }

    rc = comp1->isSubset(comp2, subsetFlag);
    if (rc == ReturnCode::RC_SUCCESS && subsetFlag) {
        ICompact* result = comp2->clone();
        if (result == nullptr) {
            LOG(logger, ReturnCode::RC_NO_MEM);
            return nullptr;
        }
        return result;
    }

    size_t dim = comp1->getDim();
    IVector* begin1 = comp1->getBegin();
    IVector* begin2 = comp2->getBegin();
    IVector* end1   = comp1->getEnd();
    IVector* end2   = comp2->getEnd();
    // TODO: remove asserts
    assert(begin1 != nullptr);
    assert(begin2 != nullptr);
    assert(end1 != nullptr);
    assert(end2 != nullptr);

    // compare begin vectors
    bool differenceFound = false;
    for (size_t i = 0; i < dim; ++i) {
        if (fabs(begin1->getCoord(i) - begin2->getCoord(i)) > tolerance) {
            if (differenceFound) {
                LOG(logger, ReturnCode::RC_INVALID_PARAMS);
                return nullptr;
            }
            differenceFound = true;
        }
    }
    bool containsFlag1 = false;
    bool containsFlag2 = false;
    rc = comp1->contains(end2, containsFlag1);
    if (rc != ReturnCode::RC_SUCCESS) {
        LOG(logger, rc);
        return nullptr;
    }
    rc = comp2->contains(end1, containsFlag2);
    if (rc != ReturnCode::RC_SUCCESS) {
        LOG(logger, rc);
        return nullptr;
    }
    if (!containsFlag1 && !containsFlag2) {
        LOG(logger, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }

    // compare end vectors
    if (!differenceFound) {
        for (size_t i = 0; i < dim; ++i) {
            if (fabs(end1->getCoord(i) - end2->getCoord(i)) > tolerance) {
                if (differenceFound) {
                    LOG(logger, ReturnCode::RC_INVALID_PARAMS);
                    return nullptr;
                }
                differenceFound = true;
            }
        }
        containsFlag1 = false;
        containsFlag2 = false;
        rc = comp1->contains(begin2, containsFlag1);
        if (rc != ReturnCode::RC_SUCCESS) {
            LOG(logger, rc);
            return nullptr;
        }
        rc = comp2->contains(begin1, containsFlag2);
        if (rc != ReturnCode::RC_SUCCESS) {
            LOG(logger, rc);
            return nullptr;
        }
        if (!containsFlag1 && !containsFlag2) {
            LOG(logger, ReturnCode::RC_INVALID_PARAMS);
            return nullptr;
        }
    }

    IVector* begin = (compare(begin1, begin2, tolerance) == VectorComparison::VC_LESSER) ?
                      begin1 : begin2;
    IVector* end   = (compare(end1, end2, tolerance) == VectorComparison::VC_BIGGER) ?
                      end1 : end2;


    ICompact* compact = ICompact::createCompact(begin, end, tolerance, logger);

    delete(begin1);
    delete(begin2);
    delete(end1);
    delete(end2);

    return compact;
}

ICompact* ICompact::intersection(ICompact const* comp1, ICompact const* comp2, double tolerance, ILogger* logger) {
    ReturnCode rc = checkCompacts(comp1, comp2);
    if (rc != ReturnCode::RC_SUCCESS) {
        LOG(logger, rc);
        return nullptr;
    }

    if (std::isnan(tolerance)) {
        LOG(logger, ReturnCode::RC_NAN);
        return nullptr;
    }

    if (tolerance < 0.0) {
        LOG(logger, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }

    bool intersects = false;
    rc = comp1->intersects(comp2, intersects);
    if (rc != ReturnCode::RC_SUCCESS) {
        LOG(logger, rc);
        return nullptr;
    }

    if (!intersects)
        return nullptr;

    IVector* begin1 = comp1->getBegin();
    IVector* begin2 = comp2->getBegin();
    IVector* end1   = comp1->getEnd();
    IVector* end2   = comp2->getEnd();
    // TODO: remove asserts
    assert(begin1 != nullptr);
    assert(begin2 != nullptr);
    assert(end1 != nullptr);
    assert(end2 != nullptr);

    IVector const* begin = (compare(begin1, begin2, tolerance) == VectorComparison::VC_BIGGER) ?
                            begin1 : begin2;
    IVector const* end   = (compare(end1, end2, tolerance)     == VectorComparison::VC_LESSER) ?
                            end1   : end2;

    ICompact* compact = ICompact::createCompact(begin, end, tolerance, logger);

    delete(begin1);
    delete(begin2);
    delete(end1);
    delete(end2);

    return compact;
}

ICompact* ICompact::convex(ICompact const* comp1, ICompact const* comp2, double tolerance, ILogger* logger) {
    ReturnCode rc = checkCompacts(comp1, comp2);
    if (rc != ReturnCode::RC_SUCCESS) {
        LOG(logger, rc);
        return nullptr;
    }

    if (std::isnan(tolerance)) {
        LOG(logger, ReturnCode::RC_NAN);
        return nullptr;
    }

    if (tolerance < 0.0) {
        LOG(logger, ReturnCode::RC_INVALID_PARAMS);
        return nullptr;
    }

    // here I got a lesson:
    // leave using goto for C, not C++
    IVector* begin1 = nullptr;
    IVector* begin2 = nullptr;
    IVector* end1   = nullptr;
    IVector* end2   = nullptr;
    size_t dim      = 0;
    double* beginData = nullptr;
    double*   endData = nullptr;
    IVector* begin    = nullptr;
    IVector*   end    = nullptr;
    ICompact* compact = nullptr;

    rc = ReturnCode::RC_UNKNOWN;
    begin1 = comp1->getBegin();
    if (begin1 == nullptr) goto convex_return;
    begin2 = comp2->getBegin();
    if (begin2 == nullptr) goto convex_delete_begin1;
    end1   = comp1->getEnd();
    if (end1   == nullptr) goto convex_delete_begin2;
    end2   = comp2->getEnd();
    if (end2   == nullptr) goto convex_delete_end1;

    rc = ReturnCode::RC_NO_MEM;
    dim = comp1->getDim();
    beginData = new(std::nothrow) double[dim];
    if (beginData == nullptr) goto convex_delete_end2;
    endData   = new(std::nothrow) double[dim];
    if (endData   == nullptr) goto convex_delete_begin_data;
    for (size_t i = 0; i < dim; ++i) {
        beginData[i] = begin1->getCoord(i) < begin2->getCoord(i) ? begin1->getCoord(i) : begin2->getCoord(i);
          endData[i] =   end1->getCoord(i) >   end2->getCoord(i) ?   end1->getCoord(i) :   end2->getCoord(i);
    }

    rc = ReturnCode::RC_UNKNOWN;
    begin = IVector::createVector(dim, beginData, logger);
    if (begin == nullptr) goto convex_delete_end_data;
    end = IVector::createVector(dim, endData, logger);
    if (end   == nullptr) goto convex_delete_begin;
    compact = ICompact::createCompact(begin, end, tolerance, logger);
    rc = compact != nullptr ? ReturnCode::RC_SUCCESS : ReturnCode::RC_UNKNOWN;

    delete end;
convex_delete_begin:
    delete begin;
convex_delete_end_data:
    delete endData;
convex_delete_begin_data:
    delete beginData;
convex_delete_end2:
    delete end2;
convex_delete_end1:
    delete end1;
convex_delete_begin2:
    delete begin2;
convex_delete_begin1:
    delete begin1;
convex_return:
    if (rc != ReturnCode::RC_SUCCESS) {
        LOG(logger, rc);
        return nullptr;
    }
    else {
        return compact;
    }
}
