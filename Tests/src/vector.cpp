#include "../include/vector.h"
#include "../include/test.h"

#ifndef LIB_IMPORT
#define LIB_IMPORT
#endif

#include "../../Logger/include/ILogger.h"
#include "../../Vector/include/IVector.h"

#include <assert.h> // assert
#include <cmath>    // fabs (C++11)
#include <new>		// nothrow
#include <iostream> // cout

// static void printVector(IVector const* v) {
// 	if (v == nullptr)
// 		return;
// 	std::cout << "vec: ";
// 	for (size_t i = 0; i < v->getDim(); ++i)
// 		std::cout << v->getCoord(i) << ' ';
// 	std::cout << '\n';
// }

bool testIVector(bool useLogger) {
	void* client    = nullptr;
	ILogger* logger = nullptr;

	if (!useLogger) {
		std::cout << "ILogger is not used\n";
	}
	else {
		client = (void*)new(std::nothrow) int;
		assert(client != nullptr);
		logger = ILogger::createLogger(client);
		assert(logger != nullptr);
		logger->setLogFile("IVector.log");
	}

    reset();
	setTestName("IVector");

	size_t dim1 = 3;
	size_t dim2 = 4;

	double* data1 = new(std::nothrow) double[dim1];
	double* data2 = new(std::nothrow) double[dim1];
	double* data3 = new(std::nothrow) double[dim1];
	double* data4 = new(std::nothrow) double[dim2];
	double* test_data_add  = new(std::nothrow) double[dim1];
	double* test_data_sub  = new(std::nothrow) double[dim1];
	double* test_data_mul  = new(std::nothrow) double[dim1];
	assert(data1 != nullptr);
	assert(data2 != nullptr);
	assert(data3 != nullptr);
	assert(data4 != nullptr);
	assert(test_data_add != nullptr);
	assert(test_data_sub != nullptr);
	assert(test_data_mul != nullptr);

	data1[0] = 1.5;
	data1[1] = 2.5;
	data1[2] = 3.5;
	data2[0] = 1.6;
	data2[1] = 2.6;
	data2[2] = 3.6;
	data3[0] = 1.7;
	data3[1] = 2.7;
	data3[2] = 3.7;
	data4[0] = 1.8;
	data4[1] = 2.8;
	data4[2] = 3.8;
	data4[3] = 4.8;

	IVector::Norm norm1 = IVector::Norm::NORM_1;
	IVector::Norm norm2 = IVector::Norm::NORM_2;
	IVector::Norm norm3 = IVector::Norm::NORM_INF;
	double tolerance1 = 0.1;
	double tolerance2 = 1.0;

	test_data_add[0] = data1[0] + data2[0];
	test_data_add[1] = data1[1] + data2[1];
	test_data_add[2] = data1[2] + data2[2];

	test_data_sub[0] = data1[0] - data2[0];
	test_data_sub[1] = data1[1] - data2[1];
	test_data_sub[2] = data1[2] - data2[2];

	test_data_mul[0] = data1[0] * 2.0;
	test_data_mul[1] = data1[1] * 2.0;
	test_data_mul[2] = data1[2] * 2.0;

	// no need to delete
	IVector* nullvec = nullptr;

	// IVector::create
	IVector* vec1 = IVector::createVector(dim1, data1, logger);
	IVector* vec2 = IVector::createVector(dim1, data2, logger);
	IVector* vec3 = IVector::createVector(dim1, data3, logger);
	IVector* vec4 = IVector::createVector(dim2, data4, logger);
	outputTest("createVector",
		vec1 != nullptr &&
		vec2 != nullptr &&
		vec3 != nullptr &&
		vec4 != nullptr,
        true);
	assert(
        vec1 != nullptr &&
        vec2 != nullptr &&
        vec3 != nullptr &&
        vec4 != nullptr);	// no need to continue

	// IVector::getDim
	outputTest("getDim",
		vec1->getDim() == dim1 &&
		vec2->getDim() == dim1 &&
		vec3->getDim() == dim1 &&
		vec4->getDim() == dim2,
        true);

	// IVector::norm
	outputTest("norm",
		vec1->norm(norm1) > 0.0 &&
		vec2->norm(norm1) > 0.0 &&
		vec3->norm(norm1) > 0.0,
        true);

	outputTest("norm",
		vec1->norm(norm2) > 0.0 &&
		vec2->norm(norm2) > 0.0 &&
		vec3->norm(norm2) > 0.0);

	outputTest("norm",
		vec1->norm(norm3) > 0.0 &&
		vec2->norm(norm3) > 0.0 &&
		vec3->norm(norm3) > 0.0);

	// IVector::equals
	bool res = false;
	outputTest("equals",
		IVector::equals(vec1, vec2, norm2, tolerance1, res, logger) == ReturnCode::RC_SUCCESS &&
        res == false,
        true);

	outputTest("equals",
		IVector::equals(vec1, vec2, norm2, tolerance2, res, logger) == ReturnCode::RC_SUCCESS &&
        res == true);

	outputTest("equals",
		IVector::equals(vec1, vec4, norm2, tolerance2, res, logger) != ReturnCode::RC_SUCCESS && 	// record will be added to logfile
        res == false);

	// IVector::clone
	IVector* vec5 = vec1->clone();
	outputTest("clone",
		vec5 != nullptr &&
		IVector::equals(vec1, vec5, norm2, tolerance1, res, logger) == ReturnCode::RC_SUCCESS &&
        res == true,
        true);

	// IVector::setCoord
	assert(vec5 != nullptr);
	outputTest("setCoord",
		vec5->setCoord(0, data2[0]) == ReturnCode::RC_SUCCESS &&
		vec5->setCoord(1, data2[1]) == ReturnCode::RC_SUCCESS &&
		vec5->setCoord(2, data2[2]) == ReturnCode::RC_SUCCESS &&
		IVector::equals(vec2, vec5, norm2, tolerance1, res, logger) == ReturnCode::RC_SUCCESS &&
        res == true,
        true);

	outputTest("setCoord",
		vec5->setCoord(3, 0.0) != ReturnCode::RC_SUCCESS);

	// IVector::add
	IVector* vec6 = IVector::add(vec1, vec2, logger);
	IVector* vec7 = IVector::createVector(dim1, test_data_add, logger);
	assert(vec7 != nullptr);
	outputTest("add",
		vec6 != nullptr &&
		IVector::equals(vec6, vec7, norm2, tolerance1, res, logger) == ReturnCode::RC_SUCCESS &&
        res == true,
        true);

	nullvec = IVector::add(vec1, vec4, logger);		// record will be added to logfile
	outputTest("add",
		nullvec == nullptr);

	// IVector::sub
	IVector* vec8 = IVector::sub(vec1, vec2, logger);
	IVector* vec9 = IVector::createVector(dim1, test_data_sub, logger);
	assert(vec9 != nullptr);
	outputTest("sub",
		vec8 != nullptr &&
		IVector::equals(vec8, vec9, norm2, tolerance1, res, logger) == ReturnCode::RC_SUCCESS &&
        res == true,
        true);

	nullvec = IVector::sub(vec1, vec4, logger);		// record will be added to logfile
	outputTest("sub",
		nullvec == nullptr);

	// IVector::mul (IVec * double)
	IVector* vec10 = IVector::mul(vec1, 2.0, logger);
	IVector* vec11 = IVector::createVector(dim1, test_data_mul, logger);
	assert(vec11 != nullptr);
	outputTest("mul (IVec * double)",
		vec10 != nullptr &&
		IVector::equals(vec10, vec11, norm2, tolerance1, res, logger) == ReturnCode::RC_SUCCESS &&
        res == true,
        true);

	// IVector::mul (IVec * IVec)
	double expect = 0.0;
	for (size_t i = 0; i < dim1; ++i) {
		expect += data1[i] * data2[i];
	}

	double result = 0.0;
	result = IVector::mul(vec1, vec2, logger);
	outputTest("mul (IVec * IVec)",
		!std::isnan(result) &&
		std::fabs(result - expect) < tolerance1, true);

	outputTest("mul (IVec * IVec)",
		std::isnan(IVector::mul(vec1, vec4)));

	delete[] data1;
	delete[] data2;
	delete[] data3;
	delete[] data4;
	delete[] test_data_add;
	delete[] test_data_sub;
	delete[] test_data_mul;

	delete vec1;
	delete vec2;
	delete vec3;
	delete vec4;
	delete vec5;
	delete vec6;
	delete vec7;
	delete vec8;
	delete vec9;
	delete vec10;
	delete vec11;

	if (logger != nullptr) {
		logger->releaseLogger(client);
	}
	if (client != nullptr) {
		delete (int*)client;
	}

    return getResult();
}
