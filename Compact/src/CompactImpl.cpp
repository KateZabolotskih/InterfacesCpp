#include "../include/ICompact.h"
#include <cmath>    // fabs (C++11)
#include <new>		// nothrow

namespace {
    /* declaration */
    class CompactImpl : public ICompact {
    private:
        size_t m_dim {0};
        IVector* m_begin {nullptr};
        IVector* m_end {nullptr};
        ILogger* m_logger {nullptr};
        double const m_tolerance;

    public:
        class IteratorImpl : public ICompact::Iterator {
        private:
            IVector const* m_begin {nullptr};
            IVector const* m_end {nullptr};
            IVector const* m_step {nullptr};
            IVector* m_current {nullptr};
            std::vector<size_t> m_direction;
            // true  - direct   traversal
            // false - reversed traversal
            bool m_traversal;
            ILogger* m_logger {nullptr};

        public:
            ReturnCode setDirection(std::vector<size_t> const& direction) override;
            ReturnCode doStep()                                           override;
            IVector* getPoint()                                     const override;

            IteratorImpl(IVector const* beg, IVector const* end, IVector const* step, std::vector<size_t> const& dir, bool traversal);
            ~IteratorImpl();
        };

        Iterator* begin(IVector const* step = nullptr) override;
        Iterator* end(IVector const* step = nullptr)   override;

        ICompact* clone()                                         const override;
        IVector* getBegin()                                       const override;
        IVector* getEnd()                                         const override;
        ReturnCode contains(IVector const* vec, bool& result)     const override;
        ReturnCode isSubset(ICompact const* comp, bool& result)   const override;
        ReturnCode intersects(ICompact const* comp, bool& result) const override;
        size_t getDim() const override;

        CompactImpl(IVector const* begin, IVector const* end, double tolerance);
        ~CompactImpl();
    };
}

/* implementation */
CompactImpl::IteratorImpl::IteratorImpl(
    IVector const* begin,
    IVector const* end,
    IVector const* step,
    std::vector<size_t> const& direction,
    bool traversal) :
    m_begin(begin),
    m_end(end),
    m_step(step),
    m_current(traversal ? begin->clone() : end->clone()),
    m_direction(direction),
    m_traversal(traversal) {
    m_logger = ILogger::createLogger(this);
    }

CompactImpl::IteratorImpl::~IteratorImpl() {
    if (m_begin != nullptr) {
        delete m_begin;
        m_begin = nullptr;
    }
    if (m_end != nullptr) {
        delete m_end;
        m_end = nullptr;
    }
    if (m_current != nullptr) {
        delete m_current;
        m_current = nullptr;
    }
    if (m_step != nullptr) {
        delete m_step;
        m_step = nullptr;
    }
    if (m_logger != nullptr) {
        m_logger->releaseLogger(this);
    }
}

// the idea of traversal is simple:
// we have vector of steps for each coordinate (m_step) and a vector of integers 0..dim-1 (m_direction).
// m_direction[0] stores number of coordinate from which we will start traversal.
// m_direction[1] stores number of the next coordinate which value will be changed
// after reaching the border of the previous coordinate m_direction[0].
// after coordinate under number <num> reaches it's border, it's value discards to the initial value
// (assigns to m_begin[<num>] for direct traversal and assigns to m_end[<num>] for reversed traversal).
// e.g. if we start direct traversal of 2D compact from point m_begin = {0.0, 0.0} to point m_end = {1.0 , 1.0}
// with m_step = {0.1, 0.5} and m_direction = {1, 0} then the sequence of visited vertices will be the following:
// (0.0, 0.0)
// (0.0, 0.5)
// (0.0, 1.0)
// (0.1, 0.0)
// (0.1, 0.5)
// (0.1, 1.0)
// (0.2, 0.0)
// (0.2, 0.5)
// ...
// (0.9, 0.0)
// (0.9, 0.5)
// (0.9, 1.0)
// (1.0, 0.0)
// (1.0, 0.5)
// (1.0, 1.0)
ReturnCode CompactImpl::IteratorImpl::doStep() {
    size_t i;
    for (i = 0; i < m_current->getDim();) {
        size_t currentDir = m_direction[i];
        double nextValue = m_current->getCoord(currentDir) + (m_traversal ? m_step->getCoord(currentDir)
                                                                         : -m_step->getCoord(currentDir));

         if (m_traversal && nextValue > m_end->getCoord(currentDir)) {
            m_current->setCoord(currentDir, m_begin->getCoord(currentDir));
            ++i;
        }
        else if (!m_traversal && nextValue < m_begin->getCoord(currentDir)){
            m_current->setCoord(currentDir, m_end->getCoord(currentDir));
            ++i;
        }
        else {
            m_current->setCoord(currentDir, nextValue);
            break;
        }
    }

    if (i >= 0 && i < m_current->getDim()) {
        return ReturnCode::RC_SUCCESS;
    }

    delete m_current;
    if (m_traversal) {
        m_current = m_end->clone();
    }
    else {
        m_current = m_begin->clone();
    }
    if (m_current == nullptr) {
        return ReturnCode::RC_UNKNOWN;
    }

    return ReturnCode::RC_OUT_OF_BOUNDS;
}

// direction is an array of integers from 0 to dim - 1,
// see description of stored numbers' semantics in doStep commentary above
ReturnCode CompactImpl::IteratorImpl::setDirection(std::vector<size_t> const& direction) {
    if (direction.size() != m_current->getDim()) {
        LOG(m_logger, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }

    // validate direction vector through checking that
    // all integers 0..dim-1 included and there is no extra stuff
    std::vector<bool> found(direction.size(), false);
    for (size_t i = 0; i < direction.size(); ++i) {
        if (direction[i] >= direction.size()) {
            LOG(m_logger, ReturnCode::RC_WRONG_DIM);
            return ReturnCode::RC_WRONG_DIM;
        }
        found[direction[i]] = true;
    }
    for (size_t i = 0; i < found.size(); ++i) {
        if (found[i] == false) {
            LOG(m_logger, ReturnCode::RC_INVALID_PARAMS);
            return ReturnCode::RC_INVALID_PARAMS;
        }
    }

    // copy new direction
    m_direction = direction;
    return ReturnCode::RC_SUCCESS;
}

IVector* CompactImpl::IteratorImpl::getPoint() const {
    return m_current->clone();
}

static IVector* buildStep(IVector const* temp, size_t dim, ILogger* logger) {
    IVector* step = nullptr;

    if (temp->getDim() != dim) {
        LOG(logger, ReturnCode::RC_WRONG_DIM);
        return nullptr;
    }

    for (size_t i = 0; i < dim; ++i) {
        // we suppose there can't be nan value, right?
        // because it's IVector's issue actually
        // same for the following similar cases
        if (temp->getCoord(i) <= 0.0) {
            LOG(logger, ReturnCode::RC_INVALID_PARAMS);
            return nullptr;
        }
    }
    step = temp->clone();

    return step;
}

static ICompact::Iterator* createIterator(IVector const* _begin, IVector const* _end, IVector const* temp, bool traversal, ILogger* logger) {
    if (_begin == nullptr || _end == nullptr || temp == nullptr) {
        LOG(logger, ReturnCode::RC_NULL_PTR);
        return nullptr;
    }
    IVector const* begin = _begin->clone();
    if (begin == nullptr) {
        LOG(logger, ReturnCode::RC_UNKNOWN);
        return nullptr;
    }
    IVector const* end = _end->clone();
    if (end == nullptr) {
        LOG(logger, ReturnCode::RC_UNKNOWN);
        delete begin;
        return nullptr;
    }
    IVector* step = buildStep(temp, begin->getDim(), logger);
    if (step == nullptr) {
        delete begin;
        delete end;
        return nullptr;
    }

    size_t dim = begin->getDim();
    std::vector<size_t> direction(dim);
    for (size_t i = 0; i < dim; ++i) {
        // if (traversal)
            direction[i] = i;
        // else
        //     direction[i] = dim - 1 - i;
    }

    ICompact::Iterator* iterator = new(std::nothrow) CompactImpl::IteratorImpl(begin, end, step, direction, traversal);
    if (iterator == nullptr) {
        delete begin;
        delete end;
        delete step;
    }

    return iterator;
}

ICompact::Iterator* CompactImpl::begin(IVector const* temp) {
    return createIterator(m_begin, m_end, temp, true, m_logger);
}

ICompact::Iterator* CompactImpl::end(IVector const* temp) {
    return createIterator(m_begin, m_end, temp, false, m_logger);
}

CompactImpl::CompactImpl(IVector const* begin, IVector const* end, double tolerance) :
    m_dim(begin->getDim()),
    m_begin(begin->clone()),
    m_end(end->clone()),
    m_tolerance(tolerance) {
    m_logger = ILogger::createLogger(this);
    }

CompactImpl::~CompactImpl() {
    m_dim = 0;

    if (m_begin != nullptr) {
        delete m_begin;
        m_begin = nullptr;
    }
    if (m_end != nullptr) {
        delete m_end;
        m_end = nullptr;
    }
    if (m_logger != nullptr) {
        m_logger->releaseLogger(this);
    }
}

IVector* CompactImpl::getBegin() const {
    return m_begin->clone();
}

IVector* CompactImpl::getEnd() const {
    return m_end->clone();
}

ReturnCode CompactImpl::contains(IVector const* vec, bool& result) const {
    result = false;
    if (vec == nullptr) {
        LOG(m_logger, ReturnCode::RC_NULL_PTR);
        return ReturnCode::RC_NULL_PTR;
    }

    if (vec->getDim() != m_dim) {
        LOG(m_logger, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }

    for (size_t i = 0; i < m_dim; ++i) {
        if (vec->getCoord(i) < m_begin->getCoord(i) ||
            vec->getCoord(i) > m_end->getCoord(i)) {
                return ReturnCode::RC_SUCCESS;
            }
    }

    result = true;
    return ReturnCode::RC_SUCCESS;
}

ReturnCode CompactImpl::isSubset(ICompact const* other, bool& result) const {
    if (other == nullptr) {
        LOG(m_logger, ReturnCode::RC_NULL_PTR);
        return ReturnCode::RC_NULL_PTR;
    }

    if (other->getDim() != m_dim) {
        LOG(m_logger, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }

    ReturnCode rc = other->contains(m_begin, result);
    if (rc != ReturnCode::RC_SUCCESS) {
        return rc;
    }

    if (!result) {
        return ReturnCode::RC_SUCCESS;
    }

    rc = other->contains(m_end, result);
    if (rc != ReturnCode::RC_SUCCESS) {
        return rc;
    }

    return ReturnCode::RC_SUCCESS;
}

ReturnCode CompactImpl::intersects(ICompact const* other, bool& result) const {
    result = false;
    if (other == nullptr) {
        LOG(m_logger, ReturnCode::RC_NULL_PTR);
        return ReturnCode::RC_NULL_PTR;
    }

    if (other->getDim() != m_dim) {
        LOG(m_logger, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }

    IVector* temp = nullptr;

    temp = other->getBegin();
    if (temp == nullptr) {
        LOG(m_logger, ReturnCode::RC_UNKNOWN);
        return ReturnCode::RC_UNKNOWN;
    }
    bool intersectsBegin = false;
    ReturnCode rc = contains(temp, intersectsBegin);
    if (rc != ReturnCode::RC_SUCCESS) {
        LOG(m_logger, rc);
        return rc;
    }
    delete temp;
    temp = nullptr;

    temp = other->getEnd();
    if (temp == nullptr) {
        LOG(m_logger, ReturnCode::RC_UNKNOWN);
        return ReturnCode::RC_UNKNOWN;
    }
    bool intersectsEnd   = false;
    rc = contains(temp, intersectsEnd);
    if (rc != ReturnCode::RC_SUCCESS) {
        LOG(m_logger, rc);
        return rc;
    }
    delete temp;
    temp = nullptr;

    result = intersectsBegin || intersectsEnd;
    return ReturnCode::RC_SUCCESS;
}

size_t CompactImpl::getDim() const {
    return m_dim;
}

ICompact* CompactImpl::clone() const {
    return ICompact::createCompact(m_begin, m_end, m_tolerance, m_logger);
}
