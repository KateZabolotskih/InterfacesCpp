#include "../include/IProblem.h"
#include <new>  // nothrow

namespace {
    /* declaration */
    class ProblemImpl : public IProblem {
    protected:
        IVector* m_params {nullptr};
        ILogger* m_logger {nullptr};

    public:
        ProblemImpl();
        ~ProblemImpl()                                                                                        override;
        ReturnCode setParams(IVector const* params)                                                           override;
        ReturnCode objectiveFunction(double& res, IVector const* args, IVector const* params = nullptr)       override;
        bool isValidCompact(ICompact const* compact)                                                    const override;
        size_t getParamsDim()                                                                           const override;
        size_t getArgsDim()                                                                             const override;

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

/* implementation */
ProblemImpl::ProblemImpl() {
    m_logger = ILogger::createLogger(this);
}

ProblemImpl::~ProblemImpl() {
    if (m_logger != nullptr) {
        m_logger->releaseLogger(this);
    }
    delete m_params;
    m_params = nullptr;
}

ReturnCode ProblemImpl::setParams(IVector const* params) {
    if (params == nullptr) {
        LOG(m_logger, ReturnCode::RC_NULL_PTR);
        return ReturnCode::RC_NULL_PTR;
    }

    if (params->getDim() != getParamsDim()) {
        LOG(m_logger, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }

    if (params->getCoord(0) == 0.0 ||
        params->getCoord(2) == 0.0) {
        LOG(m_logger, ReturnCode::RC_INVALID_PARAMS);
        return ReturnCode::RC_INVALID_PARAMS;
    }

    if (m_params != nullptr) {
        delete m_params;
        m_params = nullptr;
    }

    m_params = params->clone();
    return m_params == nullptr ? ReturnCode::RC_UNKNOWN : ReturnCode::RC_SUCCESS;
}

ReturnCode ProblemImpl::objectiveFunction(double& res, IVector const* args, IVector const* params) {
    if (args == nullptr) {
        LOG(m_logger, ReturnCode::RC_NULL_PTR);
        return ReturnCode::RC_NULL_PTR;
    }

    if (params == nullptr && m_params == nullptr) {
        LOG(m_logger, ReturnCode::RC_INIT_REQUIRED);
        return ReturnCode::RC_INIT_REQUIRED;
    }

    if (args->getDim() != getArgsDim()) {
        LOG(m_logger, ReturnCode::RC_WRONG_DIM);
        return ReturnCode::RC_WRONG_DIM;
    }

    if (params != nullptr) {
        ReturnCode rc = setParams(params);
        if (rc != ReturnCode::RC_SUCCESS) {
            return rc;
        }
    }

    // to simplify this IProblem interface implementation example
    // the objective function is given by the following paraboloid equation:
    // z = (ax + b) ^ 2  + (cy + d) ^ 2, which
    // minimum value is always 0 provided by vector (x, y) = (-b / a, -d / c)
    double tmp1 = m_params->getCoord(0) * args->getCoord(0) + m_params->getCoord(1);
    double tmp2 = m_params->getCoord(2) * args->getCoord(1) + m_params->getCoord(3);
    res = tmp1 * tmp1 + tmp2 * tmp2;
    return ReturnCode::RC_SUCCESS;
}

bool ProblemImpl::isValidCompact(ICompact const* compact) const {
    if (compact == nullptr) {
        LOG(m_logger, ReturnCode::RC_NULL_PTR);
        return false;
    }

    if (compact->getDim() != getArgsDim()) {
        return false;
    }

    // nevermind
    //    IVector* begin = nullptr;
    //    IVector* end   = nullptr;
    //    begin = compact->getBegin();
    //    if (begin == nullptr) {
    //        LOG(m_logger, ReturnCode::RC_NO_MEM);
    //        return false;
    //    }
    //    end = compact->getEnd();
    //    if (end == nullptr) {
    //        LOG(m_logger, ReturnCode::RC_NO_MEM);
    //        delete begin;
    //        return false;
    //    }
    //
    //    double begin_x = begin->getCoord(0);
    //    double begin_y = begin->getCoord(1);
    //    double end_x = end->getCoord(0);
    //    double end_y = end->getCoord(1);
    //
    //    delete begin;
    //    delete end;

    // generally speaking any 2-dimensional compact is valid for specified objective function
    // but we will limit the domain with square area: |x| <= 100, |y| <= 100
    //    if (begin_x >= -100.0 &&
    //        begin_y >= -100.0 &&
    //        end_x   <=  100.0 &&
    //        end_y   <=  100.0)
    //        return true;

    return true;
}

size_t ProblemImpl::getParamsDim() const {
    return 4;
}

size_t ProblemImpl::getArgsDim() const {
    return 2;
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
    case BrokerType::BT_PROBLEM:
        return true;
    default:
        return false;
    }
}

void* BrokerImpl::getInterfaceImpl(BrokerType type) const {
    return canCastTo(type) ? (void*)new(std::nothrow) ProblemImpl() : nullptr;
}

void BrokerImpl::release() {
    delete BrokerImpl::m_instance;
    BrokerImpl::m_instance = nullptr;
}

BrokerImpl* BrokerImpl::m_instance = new(std::nothrow) BrokerImpl();
