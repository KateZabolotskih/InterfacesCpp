#include "../include/IVector.h"

#include <stdlib.h>
#include <cmath>	// nan, isnan, sqrt, fabs (C++11)

namespace {
	/* declaration */
	class VectorImpl : public IVector {
	protected:
		size_t m_dim {0};
		double* m_data {nullptr};
		ILogger* m_logger {nullptr};

	public:
		VectorImpl(size_t dim, double* data);
		~VectorImpl() 										  override;
		IVector* clone() 								const override;
		size_t getDim() 								const override;
		ReturnCode setCoord(size_t index, double value) const override;
		double getCoord(size_t index) 					const override;
		double norm(Norm norm) 							const override;
	};
}

/* implementation */
VectorImpl::VectorImpl(size_t dim, double* data) :
	m_dim(dim), m_data(data) {
	m_logger = ILogger::createLogger(this);
}

VectorImpl::~VectorImpl() {
	if (m_data != nullptr) {
		delete[] m_data;
		m_data = nullptr;
	}
	if(m_logger != nullptr) {
        m_logger->releaseLogger(this);
	}
}

IVector* VectorImpl::clone() const {
	return createVector(m_dim, m_data, m_logger);
}

size_t VectorImpl::getDim() const {
	return m_dim;
}

ReturnCode VectorImpl::setCoord(size_t index, double value) const {
	if (index >= m_dim) {
        LOG(m_logger, ReturnCode::RC_OUT_OF_BOUNDS);
		return ReturnCode::RC_OUT_OF_BOUNDS;
	}

	if (std::isnan(value)) {
        LOG(m_logger, ReturnCode::RC_NAN);
		return ReturnCode::RC_NAN;
	}

	m_data[index] = value;
	return ReturnCode::RC_SUCCESS;
}

double VectorImpl::getCoord(size_t index) const {
	if (index >= m_dim) {
        LOG(m_logger, ReturnCode::RC_OUT_OF_BOUNDS);
		return std::nan("1");
	}
	return m_data[index];
}

double VectorImpl::norm(Norm norm) const {
	double result = 0;
	switch (norm) {
	case Norm::NORM_1:
		for (size_t i = 0; i < m_dim; ++i) {
			result += std::fabs(m_data[i]);
		}
		break;
	case Norm::NORM_2:
		for (size_t i = 0; i < m_dim; ++i) {
			result += m_data[i] * m_data[i];
		}
		result = std::sqrt(result);
		break;
	case Norm::NORM_INF:
		result = std::fabs(m_data[0]);
		for (size_t i = 1; i < m_dim; ++i) {
			if (result < std::fabs(m_data[i]))
				result = std::fabs(m_data[i]);
		}
		break;
	default:
		break;
	}

	return result;
}
