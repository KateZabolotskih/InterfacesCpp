#include "../include/ISet.h"
#include "SetImpl.cpp"
#include <new>   // nothrow
#include <cmath> // nan, isnan

static ReturnCode checkData(ISet const* set1, ISet const* set2, double tolerance) {
	if (set1 == nullptr || set2 == nullptr) {
		return ReturnCode::RC_NULL_PTR;
	}

	if (set1->getDim() == 0) {
		return ReturnCode::RC_ZERO_DIM;
	}

	if (set1->getDim() != set2->getDim()) {
		return ReturnCode::RC_WRONG_DIM;
	}

    if (std::isnan(tolerance)) {
        return ReturnCode::RC_NAN;
	}

	if (tolerance < 0) {
		return ReturnCode::RC_INVALID_PARAMS;
	}

	return ReturnCode::RC_SUCCESS;
}

ISet::~ISet() {}

ISet* ISet::createSet(ILogger* logger) {
	ISet* set = new(std::nothrow) SetImpl();
	if (set == nullptr) {
		LOG(logger, ReturnCode::RC_NO_MEM);
		return nullptr;
	}

	return set;
}

ISet* ISet::_union(ISet const* set1, ISet const* set2, IVector::Norm norm, double tolerance, ILogger* logger) {
	ReturnCode rc = checkData(set1, set2, tolerance);
	if (rc != ReturnCode::RC_SUCCESS) {
		LOG(logger, rc);
		return nullptr;
	}

	ISet* result = set1->clone();
	IVector* vec = nullptr;
	rc = ReturnCode::RC_SUCCESS;
	for (size_t i = 0; rc != ReturnCode::RC_OUT_OF_BOUNDS; ++i) {
		rc = set2->get(vec, i);
		if (vec != nullptr) {
			result->insert(vec, norm, tolerance);
			delete vec;
			vec = nullptr;
		}
	}

	return result;
}

ISet* ISet::difference(ISet const* minuend, ISet const* subtrahend, IVector::Norm norm, double tolerance, ILogger* logger) {
	ReturnCode rc = checkData(minuend, subtrahend, tolerance);
	if (rc != ReturnCode::RC_SUCCESS) {
		LOG(logger, rc);
		return nullptr;
	}

	ISet* result = minuend->clone();
	IVector* vec = nullptr;
	rc = ReturnCode::RC_SUCCESS;
	for (size_t i = 0; rc != ReturnCode::RC_OUT_OF_BOUNDS; ++i) {
		rc = subtrahend->get(vec, i);
		if (vec != nullptr) {
			result->erase(vec, norm, tolerance);
			delete vec;
			vec = nullptr;
		}
	}

	return result;
}

ISet* ISet::symmetricDifference(ISet const* set1, ISet const* set2, IVector::Norm norm, double tolerance, ILogger* logger) {
	ReturnCode rc = checkData(set1, set2, tolerance);
	if (rc != ReturnCode::RC_SUCCESS) {
		LOG(logger, rc);
		return nullptr;
	}

	ISet* u = _union(set1, set2, norm, tolerance, logger);
	ISet* i = intersection(set1, set2, norm, tolerance, logger);

	ISet* result = difference(u, i, norm, tolerance, logger);

	delete u;
	delete i;

	return result;
}

ISet* ISet::intersection(ISet const* set1, ISet const* set2, IVector::Norm norm, double tolerance, ILogger* logger) {
	ReturnCode rc = checkData(set1, set2, tolerance);
	if (rc != ReturnCode::RC_SUCCESS) {
		LOG(logger, rc);
		return nullptr;
	}

	ISet* result = set1->clone();
	IVector* ref = nullptr;
	rc = ReturnCode::RC_SUCCESS;
	for (size_t i = 0; rc != ReturnCode::RC_OUT_OF_BOUNDS; ++i) {
		rc = set1->get(ref, i);
		if (ref != nullptr) {
			size_t ind;
			if (set2->find(ref, norm, tolerance, ind) == ReturnCode::RC_SUCCESS) {
				result->erase(ind);
			}

			delete ref;
			ref = nullptr;
		}
	}

	return result;
}
