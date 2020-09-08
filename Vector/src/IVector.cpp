#include "../include/IVector.h"
#include "VectorImpl.cpp"
#include <cstring>	 // memcpy
#include <cmath>	 // nan, isnan, fabs (C++11)
#include <new>		 // nothrow
#include <assert.h>	 // assert

static ReturnCode checkData(IVector const* vec) {
	if (vec == nullptr) {
		return ReturnCode::RC_NULL_PTR;
	}

	if (vec->getDim() == 0) {
		return ReturnCode::RC_ZERO_DIM;
	}

	size_t dim = vec->getDim();
	for (size_t i = 0; i < dim; ++i) {
		if (std::isnan(vec->getCoord(i))) {
			return ReturnCode::RC_NAN;
		}
	}

	return ReturnCode::RC_SUCCESS;
}

static ReturnCode checkData(IVector const* vec1, IVector const* vec2, double tolerance = 0) {
	if (vec1 == nullptr || vec2 == nullptr) {
		return ReturnCode::RC_NULL_PTR;
	}

	if (vec1->getDim() != vec2->getDim()) {
		return ReturnCode::RC_WRONG_DIM;
	}

	size_t dim = vec1->getDim();
	for (size_t i = 0; i < dim; ++i) {
		if (std::isnan(vec1->getCoord(i)) || std::isnan(vec2->getCoord(i))) {
			return ReturnCode::RC_NAN;
		}
	}

    if (std::isnan(tolerance)) {
        return ReturnCode::RC_NAN;
    }

	if (tolerance < 0) {
		return ReturnCode::RC_INVALID_PARAMS;
	}

	return ReturnCode::RC_SUCCESS;
}

IVector::~IVector() {}

// TODO: placement new
IVector* IVector::createVector(size_t dim, double* src, ILogger* logger) {
	if (dim == 0) {
		LOG(logger, ReturnCode::RC_ZERO_DIM);
		return nullptr;
	}

	if (src == nullptr) {
		LOG(logger, ReturnCode::RC_NULL_PTR);
		return nullptr;
	}

	for (size_t i = 0; i < dim; ++i) {
		if (std::isnan(src[i])) {
			LOG(logger, ReturnCode::RC_NAN);
			return nullptr;
		}
	}

	double* dst = new(std::nothrow) double[dim];
	if (dst == nullptr) {
		LOG(logger, ReturnCode::RC_NO_MEM);
		return nullptr;
	}
	std::memcpy(dst, src, dim * sizeof(double));

	IVector* vector = new(std::nothrow) VectorImpl(dim, dst);
	if (vector == nullptr) {
		LOG(logger, ReturnCode::RC_NO_MEM);
		delete[] dst;
		return nullptr;
	}

	return vector;
}

IVector* IVector::add(IVector const* addend1, IVector const* addend2, ILogger* logger) {
	ReturnCode rc = checkData(addend1, addend2);
	if (rc != ReturnCode::RC_SUCCESS) {
		LOG(logger, rc);
		return nullptr;
	}

	size_t dim = addend1->getDim();
	double* dst = new(std::nothrow) double[dim];
	if (dst == nullptr) {
		if (logger != nullptr)
			logger->log(__FUNCTION__, ReturnCode::RC_NO_MEM);
		return nullptr;
	}

	for (size_t i = 0; i < dim; ++i) {
		dst[i] = addend1->getCoord(i) + addend2->getCoord(i);
	}

	IVector* result = new(std::nothrow) VectorImpl(dim, dst);
	if (result == nullptr) {
		LOG(logger, ReturnCode::RC_NO_MEM);
		delete[] dst;
		return nullptr;
	}

	return result;
}

IVector* IVector::sub(IVector const* minuend, IVector const* subtrahend, ILogger* logger) {
	ReturnCode rc = checkData(minuend, subtrahend);
	if (rc != ReturnCode::RC_SUCCESS) {
		LOG(logger, rc);
		return nullptr;
	}

	size_t dim = minuend->getDim();
	double* dst = new(std::nothrow) double[dim];
	if (dst == nullptr) {
		LOG(logger, ReturnCode::RC_NO_MEM);
		return nullptr;
	}

	for (size_t i = 0; i < dim; ++i) {
		dst[i] = minuend->getCoord(i) - subtrahend->getCoord(i);
	}

	IVector* result = new(std::nothrow) VectorImpl(dim, dst);
	if (result == nullptr) {
		LOG(logger, ReturnCode::RC_NO_MEM);
		delete[] dst;
		return nullptr;
	}

	return result;
}

double IVector::mul(IVector const* multiplier1, IVector const* multiplier2, ILogger* logger) {
	ReturnCode rc = checkData(multiplier1, multiplier2);
	if (rc != ReturnCode::RC_SUCCESS) {
		LOG(logger, rc);
		return std::nan("1");
	}

	size_t dim = multiplier1->getDim();
	double result = 0;
	for (size_t i = 0; i < dim; ++i) {
		result += multiplier1->getCoord(i) * multiplier2->getCoord(i);
	}

	return result;
}

IVector* IVector::mul(IVector const* multiplier, double scale, ILogger* logger) {
	ReturnCode rc = checkData(multiplier);
	if (rc != ReturnCode::RC_SUCCESS) {
		LOG(logger, rc);
		return nullptr;
	}

    if (std::isnan(scale)) {
		LOG(logger, ReturnCode::RC_NAN);
        return nullptr;
    }

	size_t dim = multiplier->getDim();
	double* dst = new(std::nothrow) double[dim];
	if (dst == nullptr) {
		LOG(logger, ReturnCode::RC_NO_MEM);
		return nullptr;
	}

	for (size_t i = 0; i < dim; ++i) {
		dst[i] = multiplier->getCoord(i) * scale;
	}

	IVector* result = new(std::nothrow) VectorImpl(dim, dst);
	if (result == nullptr) {
		LOG(logger, ReturnCode::RC_NO_MEM);
		delete[] dst;
		return nullptr;
	}

	return result;
}

ReturnCode IVector::equals(IVector const* v1, IVector const* v2, Norm norm, double tolerance, bool& result, ILogger* logger) {
	result = false;
	ReturnCode rc = checkData(v1, v2, tolerance);
	if (rc != ReturnCode::RC_SUCCESS) {
		LOG(logger, rc);
		return rc;
	}

	IVector* diff = sub(v1, v2, logger);

	if (diff == nullptr)
		return ReturnCode::RC_NULL_PTR;

    double normValue = std::fabs(diff->norm(norm));

    if (std::isnan(normValue)) {
        return ReturnCode::RC_NAN;
    }

	if (normValue < tolerance) {
        result = true;
	}

	delete diff;
	diff = nullptr;
	return ReturnCode::RC_SUCCESS;
}
