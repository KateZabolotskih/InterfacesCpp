#include "../include/ISet.h"
#include <stdlib.h>
#include <cmath>  // nan, isnan
#include <vector> // vector
#include <new>	  // nothrow

namespace {
	/* declaration */
	class SetImpl : public ISet {
	private:
		size_t m_dim {0};
		std::vector<IVector*> m_data;
		ILogger* m_logger {nullptr};

	public:
		SetImpl();
		~SetImpl() override;

		ReturnCode insert(IVector const* vector, IVector::Norm norm, double tolerance) 	override;
		ReturnCode erase(IVector const* vector, IVector::Norm norm, double tolerance) 	override;
		ReturnCode erase(size_t index) 													override;
		void clear() 																	override;

		ReturnCode find(IVector const* vector, IVector::Norm norm, double tolerance, size_t& ind) 	const override;
		ReturnCode get(IVector*& dst, size_t ind) 													const override;
		size_t getDim() 																			const override;
		size_t getSize() 																			const override;
		ISet* clone() 																				const override;
	};
}

/* implementation */
SetImpl::SetImpl() :
	m_dim(0) {
	m_logger = ILogger::createLogger(this);	
}

SetImpl::~SetImpl() {
	for (auto it = m_data.begin(); it != m_data.end(); ++it) {
		delete *it;
	}
	m_data.clear();
	m_dim = 0;

	if (m_logger != nullptr) {
		m_logger->releaseLogger(this);
	}
}

ReturnCode SetImpl::insert(IVector const* vector, IVector::Norm norm, double tolerance) {
	if (vector == nullptr) {
		LOG(m_logger, ReturnCode::RC_NULL_PTR);
		return ReturnCode::RC_NULL_PTR;
	}

	if (m_data.empty()) {
		m_data.push_back(vector->clone());
		m_dim = vector->getDim();
		return ReturnCode::RC_SUCCESS;
	}

	if (m_dim != vector->getDim()) {
		LOG(m_logger, ReturnCode::RC_WRONG_DIM);
		return ReturnCode::RC_WRONG_DIM;
	}

	if (std::isnan(tolerance)) {
		LOG(m_logger, ReturnCode::RC_NAN);
        return ReturnCode::RC_NAN;
	}

	if (tolerance < 0) {
		LOG(m_logger, ReturnCode::RC_INVALID_PARAMS);
		return ReturnCode::RC_INVALID_PARAMS;
	}

	for (auto it = m_data.begin(); it != m_data.end(); ++it) {
        bool result = false;
        IVector::equals(*it, vector, norm, tolerance, result, nullptr);
        if (result == true) {
            return ReturnCode::RC_SUCCESS;
        }
	}

	m_data.push_back(vector->clone());
	return ReturnCode::RC_SUCCESS;
}

ReturnCode SetImpl::erase(size_t index) {
	if (index >= m_data.size()) {
		LOG(m_logger, ReturnCode::RC_OUT_OF_BOUNDS);
		return ReturnCode::RC_OUT_OF_BOUNDS;
	}

	delete m_data[index];
	m_data.erase(m_data.begin() + index);

	if (m_data.empty()) {
		m_data.clear();
		m_dim = 0;
	}

	return ReturnCode::RC_SUCCESS;
}

ReturnCode SetImpl::erase(IVector const* vector, IVector::Norm norm, double tolerance) {
	size_t ind;
	bool found = false;
	ReturnCode rc = ReturnCode::RC_SUCCESS;
	while (find(vector, norm, tolerance, ind) != ReturnCode::RC_ELEM_NOT_FOUND) {
		if (rc != ReturnCode::RC_SUCCESS) {
			LOG(m_logger, rc);
			return rc;
		}
		found = true;
		rc = erase(ind);
		if (rc != ReturnCode::RC_SUCCESS) {
			return rc;
		}
	}

	return found ? ReturnCode::RC_SUCCESS : ReturnCode::RC_ELEM_NOT_FOUND;
}

ReturnCode SetImpl::get(IVector*& dst, size_t ind) const {
	if (ind >= m_data.size()) {
		LOG(m_logger, ReturnCode::RC_OUT_OF_BOUNDS);
		return ReturnCode::RC_OUT_OF_BOUNDS;
	}

	dst = m_data[ind]->clone();
	return ReturnCode::RC_SUCCESS;
}

ReturnCode SetImpl::find(IVector const* vector, IVector::Norm norm, double tolerance, size_t& ind) const {
	if (vector == nullptr) {
		LOG(m_logger, ReturnCode::RC_NULL_PTR);
		return ReturnCode::RC_NULL_PTR;
	}

	if (m_data.size() == 0 || m_dim == 0) {
		return ReturnCode::RC_ELEM_NOT_FOUND;
	}

	if (m_dim != vector->getDim()) {
		LOG(m_logger, ReturnCode::RC_WRONG_DIM);
		return ReturnCode::RC_WRONG_DIM;
	}

    if (std::isnan(tolerance)) {
		LOG(m_logger, ReturnCode::RC_NAN);
        return ReturnCode::RC_NAN;
	}

	if (tolerance < 0) {
		LOG(m_logger, ReturnCode::RC_INVALID_PARAMS);
		return ReturnCode::RC_INVALID_PARAMS;
	}

	for (auto it = m_data.begin(); it != m_data.end(); ++it) {
        bool result = false;
        IVector::equals(*it, vector, norm, tolerance, result, nullptr);
        if (result == true) {
			ind = it - m_data.begin();
			return ReturnCode::RC_SUCCESS;
        }
	}

	return ReturnCode::RC_ELEM_NOT_FOUND;
}

size_t SetImpl::getDim() const {
	return m_dim;
}

size_t SetImpl::getSize() const {
	return m_data.size();
}

ISet* SetImpl::clone() const {
	SetImpl* set = new(std::nothrow) SetImpl();
	if (set == nullptr) {
		LOG(m_logger, ReturnCode::RC_NO_MEM);
		return nullptr;
	}

	set->m_dim = m_dim;
	for (auto it = m_data.begin(); it < m_data.end(); ++it) {
		set->m_data.push_back((*it)->clone());
	}

	return set;
}

void SetImpl::clear() {
	for (auto it = m_data.begin(); it != m_data.end(); ++it) {
		delete *it;
	}
	m_data.clear();
	m_dim = 0;
}
