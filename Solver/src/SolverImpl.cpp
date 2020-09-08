#include "../include/ISolver.h"
#include <new>      // nothrow
#include <float.h>  // DBL_MAX

namespace {
    /* declaration */
    class SolverImpl : public ISolver {
    protected:
        IProblem* m_problem  {nullptr};
        ICompact* m_compact  {nullptr};
        IVector*  m_step     {nullptr};
        IVector*  m_solution {nullptr};
        ILogger*  m_logger   {nullptr};

    public:
        SolverImpl();
        ~SolverImpl()                                               override;
        ReturnCode setParams(std::string const& params)             override;
        ReturnCode setParams(IVector const* params)                 override;
        ReturnCode setProblemParams(IVector const* problemParams)   override;
        ReturnCode setCompact(ICompact const* compact)              override;
        ReturnCode setProblem(IProblem* problem)                    override;
        ReturnCode solve()                                          override;
        ReturnCode getSolution(IVector*& dst)                 const override;
        size_t getParamsDim()                                 const override;
    };

    class BrokerImpl : public IBroker {
    protected:
        static BrokerImpl* m_instance;

    public:
        static BrokerImpl* getInstance();

        BrokerImpl();
        ~BrokerImpl()                                 override;
        void release()                                override;
        bool canCastTo(BrokerType type)         const override;
        void* getInterfaceImpl(BrokerType type) const override;
    };
}

SolverImpl::SolverImpl() {
    m_logger = ILogger::createLogger(this);
}

SolverImpl::~SolverImpl() {
    m_logger->releaseLogger(this);
    delete m_compact;
    delete m_step;
    delete m_solution;
    m_problem  = nullptr;
    m_compact  = nullptr;
    m_step     = nullptr;
    m_solution = nullptr;
    m_logger   = nullptr;
}

ReturnCode SolverImpl::setParams(std::string const& params) {
    return ReturnCode::RC_SUCCESS;
}

ReturnCode SolverImpl::setParams(IVector const* params) {
    if (params == nullptr) {
        LOG(m_logger, ReturnCode::RC_NULL_PTR);
        return ReturnCode::RC_NULL_PTR;
    }

    if (m_problem == nullptr) {
        LOG(m_logger, ReturnCode::RC_INIT_REQUIRED);
        return ReturnCode::RC_INIT_REQUIRED;
    }

    if (m_problem->getArgsDim() != params->getDim()) {
        LOG(m_logger, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }

    if (m_step != nullptr) {
        delete m_step;
        m_step = nullptr;
    }

    m_step = params->clone();
    return m_step == nullptr ? ReturnCode::RC_UNKNOWN : ReturnCode::RC_SUCCESS;
}

ReturnCode SolverImpl::setProblemParams(IVector const* problemParams) {
    if (problemParams == nullptr) {
        LOG(m_logger, ReturnCode::RC_NULL_PTR);
        return ReturnCode::RC_NULL_PTR;
    }

    if (m_problem == nullptr) {
        LOG(m_logger, ReturnCode::RC_INIT_REQUIRED);
        return ReturnCode::RC_INIT_REQUIRED;
    }

    if (m_problem->getParamsDim() != problemParams->getDim()) {
        LOG(m_logger, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }

    return m_problem->setParams(problemParams);
}

ReturnCode SolverImpl::setProblem(IProblem* problem) {
    if (problem == nullptr) {
        LOG(m_logger, ReturnCode::RC_NULL_PTR);
        return ReturnCode::RC_NULL_PTR;
    }

    m_problem = problem;
    return ReturnCode::RC_SUCCESS;
}

ReturnCode SolverImpl::setCompact(ICompact const* compact) {
    if (compact== nullptr) {
        LOG(m_logger, ReturnCode::RC_NULL_PTR);
        return ReturnCode::RC_NULL_PTR;
    }

    if (m_problem == nullptr) {
        LOG(m_logger, ReturnCode::RC_INIT_REQUIRED);
        return ReturnCode::RC_INIT_REQUIRED;
    }

    if (!m_problem->isValidCompact(compact)) {
        LOG(m_logger, ReturnCode::RC_INVALID_PARAMS);
        return ReturnCode::RC_INVALID_PARAMS;
    }

    m_compact = compact->clone();
    return m_compact == nullptr ? ReturnCode::RC_UNKNOWN : ReturnCode::RC_SUCCESS;
}

ReturnCode SolverImpl::solve() {
    if (m_problem == nullptr ||
        m_compact == nullptr ||
        m_step == nullptr) {
        LOG(m_logger, ReturnCode::RC_INIT_REQUIRED);
        return ReturnCode::RC_INIT_REQUIRED;
    }

    ICompact::Iterator* iterator = m_compact->begin(m_step);
    if (iterator == nullptr) {
        LOG(m_logger, ReturnCode::RC_UNKNOWN);
        return ReturnCode::RC_UNKNOWN;
    }

    if (m_solution != nullptr) {
        delete m_solution;
        m_solution = nullptr;
    }

    double   min = DBL_MAX;
    double   tempValue = min;
    IVector* tempPoint = nullptr;
    ReturnCode rc = ReturnCode::RC_SUCCESS;
    for (tempPoint = iterator->getPoint(); rc == ReturnCode::RC_SUCCESS; rc = iterator->doStep(), tempPoint = iterator->getPoint()) {
        if (tempPoint == nullptr) {
            delete iterator;
            delete m_solution;
            m_solution = nullptr;
            LOG(m_logger, ReturnCode::RC_UNKNOWN);
            return ReturnCode::RC_UNKNOWN;
        }

        rc = m_problem->objectiveFunction(tempValue, tempPoint);
        if (rc != ReturnCode::RC_SUCCESS) {
            delete iterator;
            delete tempPoint;
            delete m_solution;
            m_solution = nullptr;
            return rc;
        }

        if (min > tempValue) {
            min = tempValue;
            delete m_solution;
            m_solution = tempPoint->clone();
        }

        delete tempPoint;
        tempPoint = nullptr;
    }

    delete iterator;
    return ReturnCode::RC_SUCCESS;
}

ReturnCode SolverImpl::getSolution(IVector*& dst) const {
    if (m_solution == nullptr) {
        LOG(m_logger, ReturnCode::RC_INIT_REQUIRED);
        return ReturnCode::RC_INIT_REQUIRED;
    }

    dst = m_solution->clone();
    return dst == nullptr ? ReturnCode::RC_UNKNOWN : ReturnCode::RC_SUCCESS;
}

size_t SolverImpl::getParamsDim() const {
    if (m_problem == nullptr) {
        LOG(m_logger, ReturnCode::RC_INIT_REQUIRED);
        return 0;
    }

    return m_problem->getArgsDim();
}

BrokerImpl::BrokerImpl() {}

BrokerImpl::~BrokerImpl() {}

BrokerImpl* BrokerImpl::getInstance() {
//    if (BrokerImpl::m_instance == nullptr) {
//        BrokerImpl::m_instance = new(std::nothrow) BrokerImpl();
//    }
    return BrokerImpl::m_instance;
}

bool BrokerImpl::canCastTo(BrokerType type) const {
    switch (type) {
    case BrokerType::BT_SOLVER:
        return true;
    default:
        return false;
    }
}

void* BrokerImpl::getInterfaceImpl(BrokerType type) const {
    return canCastTo(type) ? (void*)new(std::nothrow) SolverImpl() : nullptr;
}

void BrokerImpl::release() {
    delete BrokerImpl::m_instance;
    BrokerImpl::m_instance = nullptr;
}

BrokerImpl* BrokerImpl::m_instance = new(std::nothrow) BrokerImpl();
