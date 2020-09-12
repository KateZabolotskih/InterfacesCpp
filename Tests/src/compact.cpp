#include "../include/compact.h"
#include "../include/test.h"
#ifndef LIB_IMPORT
#define LIB_IMPORT
#endif

#include "../../Logger/include/ILogger.h"
#include "../../Vector/include/IVector.h"
#include "../../Compact/include/ICompact.h"

#include <assert.h> // assert
#include <cstddef>	// size_t
#include <vector>	// vector
#include <cmath>	// fabs (C++11)
#include <new>		// nothrow
#include <iostream> // cout

bool testICompact(bool useLogger) {
	void* client	= nullptr;
	ILogger* logger = nullptr;

	if (!useLogger) {
		std::cout << "ILogger is not used\n";
	}
	else {
		client = (void*)new(std::nothrow) int;
		assert(client != nullptr);
		logger = ILogger::createLogger(client);
		assert(logger != nullptr);
		logger->setLogFile("ICompact.log");
	}

    reset();
	setTestName("ICompact");

	size_t dim1 = 1;
	size_t dim2 = 2;
	size_t dim3 = 3;

	double* data1_beg_1 = new(std::nothrow) double[dim1];
	double* data1_end_1 = new(std::nothrow) double[dim1];
	double* data1_beg_2 = new(std::nothrow) double[dim1];
	double* data1_end_2 = new(std::nothrow) double[dim1];
	double* data2_beg_1 = new(std::nothrow) double[dim2];
	double* data2_end_1 = new(std::nothrow) double[dim2];
	double* data2_beg_2 = new(std::nothrow) double[dim2];
	double* data2_end_2 = new(std::nothrow) double[dim2];
	double* data3_beg_1 = new(std::nothrow) double[dim3];
	double* data3_end_1 = new(std::nothrow) double[dim3];
	double* data3_beg_2 = new(std::nothrow) double[dim3];
	double* data3_end_2 = new(std::nothrow) double[dim3];

	assert(data1_beg_1 != nullptr);
	assert(data1_end_1 != nullptr);
	assert(data1_beg_2 != nullptr);
	assert(data1_end_2 != nullptr);
	assert(data2_beg_1 != nullptr);
	assert(data2_end_1 != nullptr);
	assert(data2_beg_2 != nullptr);
	assert(data2_end_2 != nullptr);
	assert(data3_beg_1 != nullptr);
	assert(data3_end_1 != nullptr);
	assert(data3_beg_2 != nullptr);
	assert(data3_end_2 != nullptr);

	data1_beg_1[0] = 0.0;
	data1_end_1[0] = 1.0;
	data1_beg_2[0] = 2.0;
	data1_end_2[0] = 3.0;

	data2_beg_1[0] = 0.0;
	data2_beg_1[1] = 0.0;
	data2_end_1[0] = 1.0;
	data2_end_1[1] = 1.0;
	data2_beg_2[0] = 2.0;
	data2_beg_2[1] = 2.0;
	data2_end_2[0] = 3.0;
	data2_end_2[1] = 3.0;

	data3_beg_1[0] = 0.0;
	data3_beg_1[1] = 0.0;
	data3_beg_1[2] = 0.0;
	data3_end_1[0] = 1.0;
	data3_end_1[1] = 1.0;
	data3_end_1[2] = 1.0;
	data3_beg_2[0] = 2.0;
	data3_beg_2[1] = 2.0;
	data3_beg_2[2] = 2.0;
	data3_end_2[0] = 3.0;
	data3_end_2[1] = 3.0;
	data3_end_2[2] = 3.0;

	IVector* vec1_beg_1 = IVector::createVector(1, data1_beg_1, logger);
	IVector* vec1_end_1 = IVector::createVector(1, data1_end_1, logger);
	IVector* vec1_beg_2 = IVector::createVector(1, data1_beg_2, logger);
	IVector* vec1_end_2 = IVector::createVector(1, data1_end_2, logger);
	IVector* vec2_beg_1 = IVector::createVector(2, data2_beg_1, logger);
	IVector* vec2_end_1 = IVector::createVector(2, data2_end_1, logger);
	IVector* vec2_beg_2 = IVector::createVector(2, data2_beg_2, logger);
	IVector* vec2_end_2 = IVector::createVector(2, data2_end_2, logger);
	IVector* vec3_beg_1 = IVector::createVector(3, data3_beg_1, logger);
	IVector* vec3_end_1 = IVector::createVector(3, data3_end_1, logger);
	IVector* vec3_beg_2 = IVector::createVector(3, data3_beg_2, logger);
	IVector* vec3_end_2 = IVector::createVector(3, data3_end_2, logger);

	assert(vec1_beg_1 != nullptr);
	assert(vec1_end_1 != nullptr);
	assert(vec1_beg_2 != nullptr);
	assert(vec1_end_2 != nullptr);
	assert(vec2_beg_1 != nullptr);
	assert(vec2_end_1 != nullptr);
	assert(vec2_beg_2 != nullptr);
	assert(vec2_end_2 != nullptr);
	assert(vec3_beg_1 != nullptr);
	assert(vec3_end_1 != nullptr);
	assert(vec3_beg_2 != nullptr);
	assert(vec3_end_2 != nullptr);

	double tolerance = 1e-3;
	ICompact* comp1_1 = ICompact::createCompact(vec1_beg_1, vec1_end_1, tolerance, logger);
	ICompact* comp1_2 = ICompact::createCompact(vec1_beg_2, vec1_end_2, tolerance, logger);
	ICompact* comp2_1 = ICompact::createCompact(vec2_beg_1, vec2_end_1, tolerance, logger);
	ICompact* comp2_2 = ICompact::createCompact(vec2_beg_2, vec2_end_2, tolerance, logger);
	ICompact* comp3_1 = ICompact::createCompact(vec3_beg_1, vec3_end_1, tolerance, logger);
	ICompact* comp3_2 = ICompact::createCompact(vec3_beg_2, vec3_end_2, tolerance, logger);
	outputTest("createCompact",
		comp1_1 != nullptr &&
		comp1_2 != nullptr,
		true);

	outputTest("createCompact",
		comp2_1 != nullptr &&
		comp2_2 != nullptr);

	outputTest("createCompact",
		comp3_1 != nullptr &&
		comp3_2 != nullptr);

	assert(comp1_1 != nullptr);
	assert(comp1_2 != nullptr);
	assert(comp2_1 != nullptr);
	assert(comp2_2 != nullptr);
	assert(comp3_1 != nullptr);
	assert(comp3_2 != nullptr);
	outputTest("getDim",
		comp1_1->getDim() == 1 &&
		comp1_2->getDim() == 1,
		true);

	outputTest("getDim",
		comp2_1->getDim() == 2 &&
		comp2_2->getDim() == 2);

	outputTest("getDim",
		comp3_1->getDim() == 3 &&
		comp3_2->getDim() == 3);

	IVector* get_vec1_beg_1 = comp1_1->getBegin();
	IVector* get_vec1_end_1 = comp1_1->getEnd();
	IVector* get_vec1_beg_2 = comp1_2->getBegin();
	IVector* get_vec1_end_2 = comp1_2->getEnd();
	IVector* get_vec2_beg_1 = comp2_1->getBegin();
	IVector* get_vec2_end_1 = comp2_1->getEnd();
	IVector* get_vec2_beg_2 = comp2_2->getBegin();
	IVector* get_vec2_end_2 = comp2_2->getEnd();
	IVector* get_vec3_beg_1 = comp3_1->getBegin();
	IVector* get_vec3_end_1 = comp3_1->getEnd();
	IVector* get_vec3_beg_2 = comp3_2->getBegin();
	IVector* get_vec3_end_2 = comp3_2->getEnd();

	IVector::Norm norm = IVector::Norm::NORM_2;

	bool res;
	outputTest("getBegin",
		IVector::equals(vec1_beg_1, get_vec1_beg_1, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true																							&&
		IVector::equals(vec1_beg_2, get_vec1_beg_2, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true,
		true);

	outputTest("getBegin",
		IVector::equals(vec2_beg_1, get_vec2_beg_1, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true																							&&
		IVector::equals(vec2_beg_2, get_vec2_beg_2, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true);

	outputTest("getBegin",
		IVector::equals(vec3_beg_1, get_vec3_beg_1, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true																							&&
		IVector::equals(vec3_beg_2, get_vec3_beg_2, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true);

	outputTest("getEnd",
		IVector::equals(vec1_end_1, get_vec1_end_1, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true																							&&
		IVector::equals(vec1_end_2, get_vec1_end_2, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true,
		true);

	outputTest("getEnd",
		IVector::equals(vec2_end_1, get_vec2_end_1, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true																							&&
		IVector::equals(vec2_end_2, get_vec2_end_2, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true);

	outputTest("getEnd",
		IVector::equals(vec3_end_1, get_vec3_end_1, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true																							&&
		IVector::equals(vec3_end_2, get_vec3_end_2, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true);

	ICompact* clone_1 = comp1_1->clone();
	ICompact* clone_2 = comp2_1->clone();
	ICompact* clone_3 = comp3_1->clone();
	IVector* temp1 = clone_1->getBegin();
	IVector* temp2 = comp1_1->getBegin();
	IVector* temp3 = clone_2->getBegin();
	IVector* temp4 = comp2_1->getBegin();
	IVector* temp5 = clone_3->getBegin();
	IVector* temp6 = comp3_1->getBegin();
	IVector* temp07 = clone_1->getEnd();
	IVector* temp08 = comp1_1->getEnd();
	IVector* temp09 = clone_2->getEnd();
	IVector* temp10 = comp2_1->getEnd();
	IVector* temp11 = clone_3->getEnd();
	IVector* temp12 = comp3_1->getEnd();
	assert(temp1 != nullptr);
	assert(temp2 != nullptr);
	assert(temp3 != nullptr);
	assert(temp4 != nullptr);
	assert(temp5 != nullptr);
	assert(temp6 != nullptr);
	assert(temp07 != nullptr);
	assert(temp08 != nullptr);
	assert(temp09 != nullptr);
	assert(temp10 != nullptr);
	assert(temp11 != nullptr);
	assert(temp12 != nullptr);
	outputTest("clone",
		clone_1 != nullptr &&
		clone_1->getDim() == 1 &&
		IVector::equals(temp1, temp2, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																				&&
		IVector::equals(temp07, temp08, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true,
		true);
	outputTest("clone",
		clone_2 != nullptr &&
		clone_2->getDim() == 2 &&
		IVector::equals(temp3, temp4, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																				&&
		IVector::equals(temp09, temp10, norm, tolerance, res, logger)== ReturnCode::RC_SUCCESS 	&&
		res == true);
	outputTest("clone",
		clone_3 != nullptr &&
		clone_3->getDim() == 3 &&
		IVector::equals(temp5, temp6, norm, tolerance, res,logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																				&&
		IVector::equals(temp11, temp12, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true);

	delete temp1;
	delete temp2;
	delete temp3;
	delete temp4;
	delete temp5;
	delete temp6;
	delete temp07;
	delete temp08;
	delete temp09;
	delete temp10;
	delete temp11;
	delete temp12;

	double* contains_data1_beg_1 = new(std::nothrow) double[dim1];
	double* contains_data1_end_1 = new(std::nothrow) double[dim1];
	double* contains_data2_beg_1 = new(std::nothrow) double[dim2];
	double* contains_data2_end_1 = new(std::nothrow) double[dim2];
	double* contains_data3_beg_1 = new(std::nothrow) double[dim3];
	double* contains_data3_end_1 = new(std::nothrow) double[dim3];

	assert(contains_data1_beg_1 != nullptr);
	assert(contains_data1_end_1 != nullptr);
	assert(contains_data2_beg_1 != nullptr);
	assert(contains_data2_end_1 != nullptr);
	assert(contains_data3_beg_1 != nullptr);
	assert(contains_data3_end_1 != nullptr);

	contains_data1_beg_1[0] = 0.3;
	contains_data1_end_1[0] = 0.6;
	contains_data2_beg_1[0] = 0.3;
	contains_data2_beg_1[1] = 0.3;
	contains_data2_end_1[0] = 0.6;
	contains_data2_end_1[1] = 0.6;
	contains_data3_beg_1[0] = 0.3;
	contains_data3_beg_1[1] = 0.3;
	contains_data3_beg_1[2] = 0.3;
	contains_data3_end_1[0] = 0.6;
	contains_data3_end_1[1] = 0.6;
	contains_data3_end_1[2] = 0.6;

	IVector* contains_vec1_beg_1 = IVector::createVector(1, contains_data1_beg_1, logger);
	IVector* contains_vec1_end_1 = IVector::createVector(1, contains_data1_end_1, logger);
	IVector* contains_vec2_beg_1 = IVector::createVector(2, contains_data2_beg_1, logger);
	IVector* contains_vec2_end_1 = IVector::createVector(2, contains_data2_end_1, logger);
	IVector* contains_vec3_beg_1 = IVector::createVector(3, contains_data3_beg_1, logger);
	IVector* contains_vec3_end_1 = IVector::createVector(3, contains_data3_end_1, logger);

	assert(contains_vec1_beg_1 != nullptr);
	assert(contains_vec1_end_1 != nullptr);
	assert(contains_vec2_beg_1 != nullptr);
	assert(contains_vec2_end_1 != nullptr);
	assert(contains_vec3_beg_1 != nullptr);
	assert(contains_vec3_end_1 != nullptr);

	bool flag_contains_1_beg = false;
	bool flag_contains_1_end = false;
	bool flag_contains_2_beg = false;
	bool flag_contains_2_end = false;
	bool flag_contains_3_beg = false;
	bool flag_contains_3_end = false;
	ReturnCode rc1_1 = comp1_1->contains(contains_vec1_beg_1, flag_contains_1_beg);
	ReturnCode rc1_2 = comp1_1->contains(contains_vec1_end_1, flag_contains_1_end);
	ReturnCode rc2_1 = comp2_1->contains(contains_vec2_beg_1, flag_contains_2_beg);
	ReturnCode rc2_2 = comp2_1->contains(contains_vec2_end_1, flag_contains_2_end);
	ReturnCode rc3_1 = comp3_1->contains(contains_vec3_beg_1, flag_contains_3_beg);
	ReturnCode rc3_2 = comp3_1->contains(contains_vec3_end_1, flag_contains_3_end);
	outputTest("contains",
		rc1_1 == ReturnCode::RC_SUCCESS &&
		rc1_2 == ReturnCode::RC_SUCCESS &&
		flag_contains_1_beg == true &&
		flag_contains_1_end == true,
		true);

	outputTest("contains",
		rc2_1 == ReturnCode::RC_SUCCESS &&
		rc2_2 == ReturnCode::RC_SUCCESS &&
		flag_contains_2_beg == true &&
		flag_contains_2_end == true);

	outputTest("contains",
		rc3_1 == ReturnCode::RC_SUCCESS &&
		rc3_2 == ReturnCode::RC_SUCCESS &&
		flag_contains_3_beg == true &&
		flag_contains_3_end == true);

	double* not_contains_data1_beg_1 = new(std::nothrow) double[dim1];
	double* not_contains_data1_end_1 = new(std::nothrow) double[dim1];
	double* not_contains_data2_beg_1 = new(std::nothrow) double[dim2];
	double* not_contains_data2_end_1 = new(std::nothrow) double[dim2];
	double* not_contains_data3_beg_1 = new(std::nothrow) double[dim3];
	double* not_contains_data3_end_1 = new(std::nothrow) double[dim3];

	assert(not_contains_data1_beg_1 != nullptr);
	assert(not_contains_data1_end_1 != nullptr);
	assert(not_contains_data2_beg_1 != nullptr);
	assert(not_contains_data2_end_1 != nullptr);
	assert(not_contains_data3_beg_1 != nullptr);
	assert(not_contains_data3_end_1 != nullptr);

	not_contains_data1_beg_1[0] = 0.9;	// contains actually
	not_contains_data1_end_1[0] = 1.2;
	not_contains_data2_beg_1[0] = 0.9;	// contains actually
	not_contains_data2_beg_1[1] = 0.9;
	not_contains_data2_end_1[0] = 1.2;
	not_contains_data2_end_1[1] = 1.2;
	not_contains_data3_beg_1[0] = 0.9;	// contains actually
	not_contains_data3_beg_1[1] = 0.9;
	not_contains_data3_beg_1[2] = 0.9;
	not_contains_data3_end_1[0] = 1.2;
	not_contains_data3_end_1[1] = 1.2;
	not_contains_data3_end_1[2] = 1.2;

	IVector* not_contains_vec1_beg_1 = IVector::createVector(1, not_contains_data1_beg_1, logger);
	IVector* not_contains_vec1_end_1 = IVector::createVector(1, not_contains_data1_end_1, logger);
	IVector* not_contains_vec2_beg_1 = IVector::createVector(2, not_contains_data2_beg_1, logger);
	IVector* not_contains_vec2_end_1 = IVector::createVector(2, not_contains_data2_end_1, logger);
	IVector* not_contains_vec3_beg_1 = IVector::createVector(3, not_contains_data3_beg_1, logger);
	IVector* not_contains_vec3_end_1 = IVector::createVector(3, not_contains_data3_end_1, logger);

	assert(not_contains_vec1_beg_1 != nullptr);
	assert(not_contains_vec1_end_1 != nullptr);
	assert(not_contains_vec2_beg_1 != nullptr);
	assert(not_contains_vec2_end_1 != nullptr);
	assert(not_contains_vec3_beg_1 != nullptr);
	assert(not_contains_vec3_end_1 != nullptr);

	bool flag_not_contains_1_beg = false;
	bool flag_not_contains_1_end = false;
	bool flag_not_contains_2_beg = false;
	bool flag_not_contains_2_end = false;
	bool flag_not_contains_3_beg = false;
	bool flag_not_contains_3_end = false;
	rc1_1 = comp1_1->contains(not_contains_vec1_beg_1, flag_not_contains_1_beg);
	rc1_2 = comp1_1->contains(not_contains_vec1_end_1, flag_not_contains_1_end);
	rc2_1 = comp2_1->contains(not_contains_vec2_beg_1, flag_not_contains_2_beg);
	rc2_2 = comp2_1->contains(not_contains_vec2_end_1, flag_not_contains_2_end);
	rc3_1 = comp3_1->contains(not_contains_vec3_beg_1, flag_not_contains_3_beg);
	rc3_2 = comp3_1->contains(not_contains_vec3_end_1, flag_not_contains_3_end);
	outputTest("contains",
		rc1_1 == ReturnCode::RC_SUCCESS &&
		rc1_2 == ReturnCode::RC_SUCCESS &&
		flag_not_contains_1_beg == true &&
		flag_not_contains_1_end == false);

	outputTest("contains",
		rc2_1 == ReturnCode::RC_SUCCESS &&
		rc2_2 == ReturnCode::RC_SUCCESS &&
		flag_not_contains_2_beg == true &&
		flag_not_contains_2_end == false);

	outputTest("contains",
		rc3_1 == ReturnCode::RC_SUCCESS &&
		rc3_2 == ReturnCode::RC_SUCCESS &&
		flag_not_contains_3_beg == true &&
		flag_not_contains_3_end == false);

	ICompact* subset_comp1_1 = ICompact::createCompact(contains_vec1_beg_1, contains_vec1_end_1, tolerance, logger);
	ICompact* subset_comp2_1 = ICompact::createCompact(contains_vec2_beg_1, contains_vec2_end_1, tolerance, logger);
	ICompact* subset_comp3_1 = ICompact::createCompact(contains_vec3_beg_1, contains_vec3_end_1, tolerance, logger);
	ICompact* not_subset_comp1_1 = ICompact::createCompact(not_contains_vec1_beg_1, not_contains_vec1_end_1, tolerance, logger);
	ICompact* not_subset_comp2_1 = ICompact::createCompact(not_contains_vec2_beg_1, not_contains_vec2_end_1, tolerance, logger);
	ICompact* not_subset_comp3_1 = ICompact::createCompact(not_contains_vec3_beg_1, not_contains_vec3_end_1, tolerance, logger);

	assert(subset_comp1_1 	  != nullptr);
	assert(subset_comp2_1 	  != nullptr);
	assert(subset_comp3_1 	  != nullptr);
	assert(not_subset_comp1_1 != nullptr);
	assert(not_subset_comp2_1 != nullptr);
	assert(not_subset_comp3_1 != nullptr);

	bool flag_subset_1 = false;
	bool flag_subset_2 = false;
	bool flag_subset_3 = false;
	bool flag_not_subset_1 = false;
	bool flag_not_subset_2 = false;
	bool flag_not_subset_3 = false;
	rc1_1 = subset_comp1_1->isSubset(comp1_1, flag_subset_1);
	rc2_1 = subset_comp2_1->isSubset(comp2_1, flag_subset_2);
	rc3_1 = subset_comp3_1->isSubset(comp3_1, flag_subset_3);
	rc1_2 = not_subset_comp1_1->isSubset(comp1_1, flag_not_subset_1);
	rc2_2 = not_subset_comp2_1->isSubset(comp2_1, flag_not_subset_2);
	rc3_2 = not_subset_comp3_1->isSubset(comp3_1, flag_not_subset_3);
	outputTest("isSubset",
		rc1_1 == ReturnCode::RC_SUCCESS &&
		rc1_2 == ReturnCode::RC_SUCCESS &&
		flag_subset_1 &&
		!flag_not_subset_1,
		true);
	outputTest("isSubset",
		rc2_1 == ReturnCode::RC_SUCCESS &&
		rc2_2 == ReturnCode::RC_SUCCESS &&
		flag_subset_2 &&
		!flag_not_subset_2);
	outputTest("isSubset",
		rc3_1 == ReturnCode::RC_SUCCESS &&
		rc3_2 == ReturnCode::RC_SUCCESS &&
		flag_subset_3 &&
		!flag_not_subset_3);

	ICompact* intersects_comp1_1 = not_subset_comp1_1->clone();
	ICompact* intersects_comp2_1 = not_subset_comp2_1->clone();
	ICompact* intersects_comp3_1 = not_subset_comp3_1->clone();

	ICompact* not_intersects_comp1_1 = ICompact::createCompact(vec1_beg_2, vec1_end_2, tolerance, logger);
	ICompact* not_intersects_comp2_1 = ICompact::createCompact(vec2_beg_2, vec2_end_2, tolerance, logger);
	ICompact* not_intersects_comp3_1 = ICompact::createCompact(vec3_beg_2, vec3_end_2, tolerance, logger);

	assert(intersects_comp1_1 != nullptr);
	assert(intersects_comp2_1 != nullptr);
	assert(intersects_comp3_1 != nullptr);
	assert(not_intersects_comp1_1 != nullptr);
	assert(not_intersects_comp2_1 != nullptr);
	assert(not_intersects_comp3_1 != nullptr);

	bool flag_intersects_1 = false;
	bool flag_intersects_2 = false;
	bool flag_intersects_3 = false;
	bool flag_not_intersects_1 = false;
	bool flag_not_intersects_2 = false;
	bool flag_not_intersects_3 = false;
	rc1_1 = comp1_1->intersects(intersects_comp1_1, flag_intersects_1);
	rc2_1 = comp2_1->intersects(intersects_comp2_1, flag_intersects_2);
	rc3_1 = comp3_1->intersects(intersects_comp3_1, flag_intersects_3);
	rc1_2 = comp1_1->intersects(not_intersects_comp1_1, flag_not_intersects_1);
	rc2_2 = comp2_1->intersects(not_intersects_comp2_1, flag_not_intersects_2);
	rc3_2 = comp3_1->intersects(not_intersects_comp3_1, flag_not_intersects_3);
	outputTest("intersects",
		rc1_1 == ReturnCode::RC_SUCCESS &&
		rc1_2 == ReturnCode::RC_SUCCESS &&
		flag_intersects_1 &&
		!flag_not_intersects_1,
		true);

	outputTest("intersects",
		rc2_1 == ReturnCode::RC_SUCCESS &&
		rc2_2 == ReturnCode::RC_SUCCESS &&
		flag_intersects_2 &&
		!flag_not_intersects_2);

	outputTest("intersects",
		rc3_1 == ReturnCode::RC_SUCCESS &&
		rc3_2 == ReturnCode::RC_SUCCESS &&
		flag_intersects_3 &&
		!flag_not_intersects_3);

	ICompact* intersection_1 = ICompact::intersection(comp1_1, not_subset_comp1_1, tolerance, logger);
	ICompact* intersection_2 = ICompact::intersection(comp2_1, not_subset_comp2_1, tolerance, logger);
	ICompact* intersection_3 = ICompact::intersection(comp3_1, not_subset_comp3_1, tolerance, logger);
	temp1 = intersection_1->getBegin();
	temp2 = intersection_2->getBegin();
	temp3 = intersection_3->getBegin();
	temp4 = intersection_1->getEnd();
	temp5 = intersection_2->getEnd();
	temp6 = intersection_3->getEnd();
	assert(temp1 != nullptr);
	assert(temp2 != nullptr);
	assert(temp3 != nullptr);
	assert(temp4 != nullptr);
	assert(temp5 != nullptr);
	assert(temp6 != nullptr);
	outputTest("intersection",
		intersection_1 != nullptr &&
		intersection_2 != nullptr &&
		intersection_3 != nullptr &&
		IVector::equals(not_contains_vec1_beg_1, temp1, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true																								&&
		IVector::equals(not_contains_vec2_beg_1, temp2, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true																								&&
		IVector::equals(not_contains_vec3_beg_1, temp3, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true																								&&
		IVector::equals(vec1_end_1, temp4, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 				&&
		res == true																								&&
		IVector::equals(vec2_end_1, temp5, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 				&&
		res == true																								&&
		IVector::equals(vec3_end_1, temp6, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 				&&
		res == true,
		true);
	temp1 = nullptr;
	temp2 = nullptr;
	temp3 = nullptr;
	temp4 = nullptr;
	temp5 = nullptr;
	temp6 = nullptr;

	// no need to delete
	ICompact* null_comp1 = ICompact::intersection(comp1_1, comp1_2, tolerance, logger);
	ICompact* null_comp2 = ICompact::intersection(comp2_1, comp2_2, tolerance, logger);
	ICompact* null_comp3 = ICompact::intersection(comp3_1, comp3_2, tolerance, logger);
	outputTest("intersection",
		null_comp1 == nullptr &&
		null_comp2 == nullptr &&
		null_comp3 == nullptr);

	ICompact* convex_1 = ICompact::convex(comp1_1, comp1_2, tolerance, logger);
	ICompact* convex_2 = ICompact::convex(comp2_1, comp2_2, tolerance, logger);
	ICompact* convex_3 = ICompact::convex(comp3_1, comp3_2, tolerance, logger);
    assert(convex_1 != nullptr);
    assert(convex_2 != nullptr);
    assert(convex_3 != nullptr);
	temp1 = convex_1->getBegin();
	temp2 = convex_2->getBegin();
	temp3 = convex_3->getBegin();
	temp4 = convex_1->getEnd();
	temp5 = convex_2->getEnd();
	temp6 = convex_3->getEnd();
	assert(temp1 != nullptr);
	assert(temp2 != nullptr);
	assert(temp3 != nullptr);
	assert(temp4 != nullptr);
	assert(temp5 != nullptr);
	assert(temp6 != nullptr);
	outputTest("convex",
		convex_1 != nullptr &&
		convex_2 != nullptr &&
		convex_3 != nullptr &&
		IVector::equals(vec1_beg_1, temp1, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																					&&
		IVector::equals(vec2_beg_1, temp2, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																					&&
		IVector::equals(vec3_beg_1, temp3, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																					&&
		IVector::equals(vec1_end_2, temp4, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																					&&
		IVector::equals(vec2_end_2, temp5, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																					&&
		IVector::equals(vec3_end_2, temp6, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true,
		true);
	temp1 = nullptr;
	temp2 = nullptr;
	temp3 = nullptr;
	temp4 = nullptr;
	temp5 = nullptr;
	temp6 = nullptr;

	ICompact* convex_1_from_intersected = ICompact::convex(not_subset_comp1_1, comp1_1, tolerance, logger);
	ICompact* convex_2_from_intersected = ICompact::convex(not_subset_comp2_1, comp2_1, tolerance, logger);
	ICompact* convex_3_from_intersected = ICompact::convex(not_subset_comp3_1, comp3_1, tolerance, logger);
	temp1 = convex_1_from_intersected->getBegin();
	temp2 = convex_2_from_intersected->getBegin();
	temp3 = convex_3_from_intersected->getBegin();
	temp4 = convex_1_from_intersected->getEnd();
	temp5 = convex_2_from_intersected->getEnd();
	temp6 = convex_3_from_intersected->getEnd();
	assert(temp1 != nullptr);
	assert(temp2 != nullptr);
	assert(temp3 != nullptr);
	assert(temp4 != nullptr);
	assert(temp5 != nullptr);
	assert(temp6 != nullptr);
	outputTest("convex",
		convex_1_from_intersected != nullptr &&
		convex_2_from_intersected != nullptr &&
		convex_3_from_intersected != nullptr &&
		IVector::equals(vec1_beg_1, temp1, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																					&&
		IVector::equals(vec2_beg_1, temp2, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																					&&
		IVector::equals(vec3_beg_1, temp3, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																					&&
		IVector::equals(not_contains_vec1_end_1, temp4, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true																								&&
		IVector::equals(not_contains_vec2_end_1, temp5, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true																								&&
		IVector::equals(not_contains_vec3_end_1, temp6, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true);
	temp1 = nullptr;
	temp2 = nullptr;
	temp3 = nullptr;
	temp4 = nullptr;
	temp5 = nullptr;
	temp6 = nullptr;


	double* add_test_data_2_1_beg = new(std::nothrow) double[dim2];
	double* add_test_data_2_1_end = new(std::nothrow) double[dim2];
	double* add_test_data_2_2_beg = new(std::nothrow) double[dim2];
	double* add_test_data_2_2_end = new(std::nothrow) double[dim2];

	double* add_test_data_3_1_beg = new(std::nothrow) double[dim2];
	double* add_test_data_3_1_end = new(std::nothrow) double[dim2];
	double* add_test_data_3_2_beg = new(std::nothrow) double[dim2];
	double* add_test_data_3_2_end = new(std::nothrow) double[dim2];

	assert(add_test_data_2_1_beg != nullptr);
	assert(add_test_data_2_1_end != nullptr);
	assert(add_test_data_2_2_beg != nullptr);
	assert(add_test_data_2_2_end != nullptr);
	assert(add_test_data_3_1_beg != nullptr);
	assert(add_test_data_3_1_end != nullptr);
	assert(add_test_data_3_2_beg != nullptr);
	assert(add_test_data_3_2_end != nullptr);

	add_test_data_2_1_beg[0] = 1.0;
	add_test_data_2_1_beg[1] = 1.0;
	add_test_data_2_1_end[0] = 3.0;
	add_test_data_2_1_end[1] = 2.0;
	add_test_data_2_2_beg[0] = 2.0;
	add_test_data_2_2_beg[1] = 1.0;
	add_test_data_2_2_end[0] = 4.0;
	add_test_data_2_2_end[1] = 2.0;

	add_test_data_3_1_beg[0] = 1.0;
	add_test_data_3_1_beg[1] = 1.0;
	add_test_data_3_1_end[0] = 4.0;
	add_test_data_3_1_end[1] = 2.0;
	add_test_data_3_2_beg[0] = 2.0;
	add_test_data_3_2_beg[1] = 1.0;
	add_test_data_3_2_end[0] = 3.0;
	add_test_data_3_2_end[1] = 2.0;

	IVector* add_vec_2_1_beg = IVector::createVector(2, add_test_data_2_1_beg, logger);
	IVector* add_vec_2_1_end = IVector::createVector(2, add_test_data_2_1_end, logger);
	IVector* add_vec_2_2_beg = IVector::createVector(2, add_test_data_2_2_beg, logger);
	IVector* add_vec_2_2_end = IVector::createVector(2, add_test_data_2_2_end, logger);

	IVector* add_vec_3_1_beg = IVector::createVector(2, add_test_data_3_1_beg, logger);
	IVector* add_vec_3_1_end = IVector::createVector(2, add_test_data_3_1_end, logger);
	IVector* add_vec_3_2_beg = IVector::createVector(2, add_test_data_3_2_beg, logger);
	IVector* add_vec_3_2_end = IVector::createVector(2, add_test_data_3_2_end, logger);

	assert(add_vec_2_1_beg != nullptr);
	assert(add_vec_2_1_end != nullptr);
	assert(add_vec_2_2_beg != nullptr);
	assert(add_vec_2_2_end != nullptr);

	assert(add_vec_3_1_beg != nullptr);
	assert(add_vec_3_1_end != nullptr);
	assert(add_vec_3_2_beg != nullptr);
	assert(add_vec_3_2_end != nullptr);

	ICompact* test_add_2_1 = ICompact::createCompact(add_vec_2_1_beg, add_vec_2_1_end, tolerance, logger);
	ICompact* test_add_2_2 = ICompact::createCompact(add_vec_2_2_beg, add_vec_2_2_end, tolerance, logger);
	ICompact* test_add_3_1 = ICompact::createCompact(add_vec_3_1_beg, add_vec_3_1_end, tolerance, logger);
	ICompact* test_add_3_2 = ICompact::createCompact(add_vec_3_2_beg, add_vec_3_2_end, tolerance, logger);

	assert(test_add_2_1 != nullptr);
	assert(test_add_2_2 != nullptr);
	assert(test_add_3_1 != nullptr);
	assert(test_add_3_2 != nullptr);

	ICompact* add_1 = ICompact::_union(comp1_1, not_subset_comp1_1, tolerance, logger);
	ICompact* add_2 = ICompact::_union(test_add_2_1, test_add_2_2,  tolerance, logger);
	ICompact* add_3 = ICompact::_union(test_add_3_1, test_add_3_2,  tolerance, logger);
	temp1 = comp1_1->getBegin();
	temp2 = add_1->getBegin();
	temp3 = test_add_2_1->getBegin();
	temp4 = add_2->getBegin();
	temp5 = test_add_3_1->getBegin();
	temp6 = add_3->getBegin();
	assert(temp1 != nullptr);
	assert(temp2 != nullptr);
	assert(temp3 != nullptr);
	assert(temp4 != nullptr);
	assert(temp5 != nullptr);
	assert(temp6 != nullptr);
	temp07 = not_subset_comp1_1->getEnd();
	temp08 = add_1->getEnd();
	temp09 = test_add_2_2->getEnd();
	temp10 = add_2->getEnd();
	temp11 = test_add_3_1->getEnd();
	temp12 = add_3->getEnd();
	assert(temp07 != nullptr);
	assert(temp08 != nullptr);
	assert(temp09 != nullptr);
	assert(temp10 != nullptr);
	assert(temp11 != nullptr);
	assert(temp12 != nullptr);

	outputTest("union",
		add_1 != nullptr &&
		IVector::equals(temp1, temp2, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS   &&
		res == true																				&&
		IVector::equals(temp07, temp08, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true,
		true);

	outputTest("union",
		add_2 != nullptr &&
		IVector::equals(temp3, temp4, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																				&&
		IVector::equals(temp09, temp10, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true);

	outputTest("union",
		add_3 != nullptr &&
		IVector::equals(temp5, temp6, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																				&&
		IVector::equals(temp11, temp12, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true);
	temp1 = nullptr;
	temp2 = nullptr;
	temp3 = nullptr;
	temp4 = nullptr;
	temp5 = nullptr;
	temp6 = nullptr;
	temp07 = nullptr;
	temp08 = nullptr;
	temp09 = nullptr;
	temp10 = nullptr;
	temp11 = nullptr;
	temp12 = nullptr;

	double* step_1 = new (std::nothrow) double[dim1];
	double* step_2 = new (std::nothrow) double[dim2];
	double* step_3 = new (std::nothrow) double[dim3];
	double* step_end_1 = new (std::nothrow) double[dim1];
	double* step_end_2 = new (std::nothrow) double[dim2];
	double* step_end_3 = new (std::nothrow) double[dim3];
	assert(step_1 != nullptr);
	assert(step_2 != nullptr);
	assert(step_3 != nullptr);
	assert(step_end_1 != nullptr);
	assert(step_end_2 != nullptr);
	assert(step_end_3 != nullptr);

	double step = 0.1;

	step_1[0] = step;
	step_2[0] = step;
	step_2[1] = step;
	step_3[0] = step;
	step_3[1] = step;
	step_3[2] = step;

	step_end_1[0] = step;
	step_end_2[0] = step;
	step_end_2[1] = step;
	step_end_3[0] = step;
	step_end_3[1] = step;
	step_end_3[2] = step;

	IVector* vstep_1 = IVector::createVector(dim1, step_1, logger);
	IVector* vstep_2 = IVector::createVector(dim2, step_2, logger);
	IVector* vstep_3 = IVector::createVector(dim3, step_3, logger);
	IVector* vstep_end_1 = IVector::createVector(dim1, step_end_1, logger);
	IVector* vstep_end_2 = IVector::createVector(dim2, step_end_2, logger);
	IVector* vstep_end_3 = IVector::createVector(dim3, step_end_3, logger);

	assert(vstep_1 != nullptr);
	assert(vstep_2 != nullptr);
	assert(vstep_3 != nullptr);
	assert(vstep_end_1 != nullptr);
	assert(vstep_end_2 != nullptr);
	assert(vstep_end_3 != nullptr);

	ICompact::Iterator* it_1 = comp1_1->begin(vstep_1);
	ICompact::Iterator* it_2 = comp2_1->begin(vstep_2);
	ICompact::Iterator* it_3 = comp3_1->begin(vstep_3);
	outputTest("begin",
		it_1 != nullptr &&
		it_2 != nullptr &&
		it_3 != nullptr,
		true);

	// delete it_1;
	// delete it_2;
	// delete it_3;
	// it_1 = nullptr;
	// it_2 = nullptr;
	// it_3 = nullptr;
	// it_1 = comp1_1->begin();
	// it_2 = comp2_1->begin();
	// it_3 = comp3_1->begin();
	// outputTest("begin",
	// 	it_1 != nullptr &&
	// 	it_2 != nullptr &&
	// 	it_3 != nullptr);

    assert(comp1_1 != nullptr);
    assert(comp2_1 != nullptr);
    assert(comp3_1 != nullptr);

	ICompact::Iterator* it_end_1 = comp1_1->end(vstep_end_1);
	ICompact::Iterator* it_end_2 = comp2_1->end(vstep_end_2);
	ICompact::Iterator* it_end_3 = comp3_1->end(vstep_end_3);
	outputTest("end",
		it_end_1 != nullptr &&
		it_end_2 != nullptr &&
		it_end_3 != nullptr,
		true);

	// delete it_end_1;
	// delete it_end_2;
	// delete it_end_3;
	// it_end_1 = nullptr;
	// it_end_2 = nullptr;
	// it_end_3 = nullptr;
	// it_end_1 = comp1_1->end();
	// it_end_2 = comp2_1->end();
	// it_end_3 = comp3_1->end();
	// outputTest("end",
	// 	it_end_1 != nullptr &&
	// 	it_end_2 != nullptr &&
	// 	it_end_3 != nullptr);

	temp1 = it_1->getPoint();
	temp2 = it_2->getPoint();
	temp3 = it_3->getPoint();
	temp4 = it_end_1->getPoint();
	temp5 = it_end_2->getPoint();
	temp6 = it_end_3->getPoint();
	temp07 = comp1_1->getBegin();
	temp08 = comp2_1->getBegin();
	temp09 = comp3_1->getBegin();
	temp10 = comp1_1->getEnd();
	temp11 = comp2_1->getEnd();
	temp12 = comp3_1->getEnd();
	assert(temp1 != nullptr);
	assert(temp2 != nullptr);
	assert(temp3 != nullptr);
	assert(temp4 != nullptr);
	assert(temp5 != nullptr);
	assert(temp6 != nullptr);
	assert(temp07 != nullptr);
	assert(temp08 != nullptr);
	assert(temp09 != nullptr);
	assert(temp10 != nullptr);
	assert(temp11 != nullptr);
	assert(temp12 != nullptr);
	outputTest("Iterator::getPoint",
		IVector::equals(temp1, temp07, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																				&&
		IVector::equals(temp2, temp08, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																				&&
		IVector::equals(temp3, temp09, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																				&&
		IVector::equals(temp4, temp10, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																				&&
		IVector::equals(temp5, temp11, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS 	&&
		res == true																				&&
		IVector::equals(temp6, temp12, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS  &&
		res == true,
		true);
	delete temp1;
	delete temp2;
	delete temp3;
	delete temp4;
	delete temp5;
	delete temp6;
	delete temp07;
	delete temp08;
	delete temp09;
	delete temp10;
	delete temp11;
	delete temp12;

	std::vector<size_t> dir_1(dim1);
	std::vector<size_t> dir_2(dim2);
	std::vector<size_t> dir_3(dim3);

	dir_1[0] = 0;

	dir_2[0] = 1;
	dir_2[1] = 0;

	dir_3[0] = 2;
	dir_3[1] = 1;
	dir_3[2] = 0;

	outputTest("Iterator::setDir",
		it_1->setDirection(dir_1) == ReturnCode::RC_SUCCESS &&
		it_2->setDirection(dir_2) == ReturnCode::RC_SUCCESS &&
		it_3->setDirection(dir_3) == ReturnCode::RC_SUCCESS,
		true);

	dir_1[0] = 0;

	dir_2[0] = 0;
	dir_2[1] = 1;

	dir_3[0] = 0;
	dir_3[1] = 1;
	dir_3[2] = 2;

	outputTest("Iterator::setDir",
		it_1->setDirection(dir_1) == ReturnCode::RC_SUCCESS &&
		it_2->setDirection(dir_2) == ReturnCode::RC_SUCCESS &&
		it_3->setDirection(dir_3) == ReturnCode::RC_SUCCESS);

	outputTest("Iterator::doStep",
		it_1->doStep() == ReturnCode::RC_SUCCESS &&
		it_2->doStep() == ReturnCode::RC_SUCCESS &&
		it_3->doStep() == ReturnCode::RC_SUCCESS,
		true);

	outputTest("Iterator::doStep",
		it_end_1->doStep() == ReturnCode::RC_SUCCESS &&
		it_end_2->doStep() == ReturnCode::RC_SUCCESS &&
		it_end_3->doStep() == ReturnCode::RC_SUCCESS);

	// temp1 = it_1->getPoint();
	// temp2 = it_2->getPoint();
	// temp3 = it_3->getPoint();
	// temp4 = it_end_1->getPoint();
	// temp5 = it_end_2->getPoint();
	// temp6 = it_end_3->getPoint();
	// assert(temp1 != nullptr);
	// assert(temp2 != nullptr);
	// assert(temp3 != nullptr);
	// assert(temp4 != nullptr);
	// assert(temp5 != nullptr);
	// assert(temp6 != nullptr);

	// double eps = 1e-3;
	// outputTest("Iterator::doStep",
	// 	std::fabs(temp1->getCoord(0) - step) < eps &&
	// 	std::fabs(temp2->getCoord(0) - step) < eps &&
	// 	temp2->getCoord(1) == 0.0 &&
	// 	std::fabs(temp3->getCoord(0) - step) < eps &&
	// 	temp3->getCoord(1) == 0.0 &&
	// 	temp3->getCoord(2) == 0.0);

	// outputTest("Iterator::doStep",
	// 	std::fabs(temp4->getCoord(0) - (1.0 - step)) < eps &&
	// 	std::fabs(temp5->getCoord(1) - (1.0 - step)) < eps &&
	// 	temp5->getCoord(0) == 1.0 &&
	// 	std::fabs(temp6->getCoord(2) - (1.0 - step)) < eps &&
	// 	temp6->getCoord(1) == 1.0 &&
	// 	temp6->getCoord(0) == 1.0);

	// delete temp1;
	// delete temp2;
	// delete temp3;
	// delete temp4;
	// delete temp5;
	// delete temp6;

	ReturnCode step_rc;

	/* uncomment code below to print iteration through Compact */

	//size_t steps;
	//std::cout << "\nDIRECT TRAVERSAL\n";
	// step_rc = ReturnCode::RC_SUCCESS;
	// steps = 0;
	// while (step_rc == ReturnCode::RC_SUCCESS) {
	// 	step_rc = it_2->doStep();
	// 	++steps;
	// 	printVector(it_2->getPoint());
	// }
	// std::cout << "\n steps:" << steps << '\n';
	//std::cout << "\nREVERSE TRAVERSAL\n";
	// step_rc = ReturnCode::RC_SUCCESS;
	// steps = 0;
	// while (step_rc == ReturnCode::RC_SUCCESS) {
	// 	step_rc = it_end_2->doStep();
	// 	++steps;
	// 	printVector(it_end_2->getPoint());
	// }
	// std::cout << "\n steps:" << steps << '\n';

	temp1 = comp1_1->getEnd();
	temp2 = comp2_1->getEnd();
	temp3 = comp3_1->getEnd();
	temp4 = comp1_1->getBegin();
	temp5 = comp2_1->getBegin();
	temp6 = comp3_1->getBegin();
	assert(temp1 != nullptr);
	assert(temp2 != nullptr);
	assert(temp3 != nullptr);
	assert(temp4 != nullptr);
	assert(temp5 != nullptr);
	assert(temp6 != nullptr);

	while ((step_rc = it_1->doStep()) == ReturnCode::RC_SUCCESS);
	temp07 = it_1->getPoint();
	assert(temp07 != nullptr);
	outputTest("Iterator::doStep",
		step_rc != ReturnCode::RC_SUCCESS 																			 &&
		IVector::equals(temp1, temp07, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true);

	while ((step_rc = it_2->doStep()) == ReturnCode::RC_SUCCESS);
	temp08 = it_2->getPoint();
	assert(temp08 != nullptr);
	outputTest("Iterator::doStep",
		step_rc != ReturnCode::RC_SUCCESS 																			 &&
		IVector::equals(temp2, temp08, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true);

	while ((step_rc = it_3->doStep()) == ReturnCode::RC_SUCCESS);
	temp09 = it_3->getPoint();
	assert(temp09 != nullptr);
	outputTest("Iterator::doStep",
		step_rc != ReturnCode::RC_SUCCESS																			 &&
		IVector::equals(temp3, temp09, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true);

	while ((step_rc = it_end_1->doStep()) == ReturnCode::RC_SUCCESS);
	temp10 = it_end_1->getPoint();
	assert(temp10 != nullptr);
	outputTest("Iterator::doStep",
		step_rc != ReturnCode::RC_SUCCESS																				   &&
		IVector::equals(temp4, temp10, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true);

	while ((step_rc = it_end_2->doStep()) == ReturnCode::RC_SUCCESS);
	temp11 = it_end_2->getPoint();
	assert(temp11 != nullptr);
	outputTest("Iterator::doStep",
		step_rc != ReturnCode::RC_SUCCESS 																				   &&
		IVector::equals(temp5, temp11, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true);

	while ((step_rc = it_end_3->doStep()) == ReturnCode::RC_SUCCESS);
	temp12 = it_end_3->getPoint();
	assert(temp12 != nullptr);
	outputTest("Iterator::doStep",
		step_rc != ReturnCode::RC_SUCCESS 																				   &&
		IVector::equals(temp6, temp12, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
		res == true);

	delete temp1;
	delete temp2;
	delete temp3;
	delete temp4;
	delete temp5;
	delete temp6;
	delete temp07;
	delete temp08;
	delete temp09;
	delete temp10;
	delete temp11;
	delete temp12;

	delete[] step_1;
	delete[] step_2;
	delete[] step_3;
	delete[] step_end_1;
	delete[] step_end_2;
	delete[] step_end_3;

	delete vstep_1;
	delete vstep_2;
	delete vstep_3;
	delete vstep_end_1;
	delete vstep_end_2;
	delete vstep_end_3;

	delete it_1;
	delete it_2;
	delete it_3;
	delete it_end_1;
	delete it_end_2;
	delete it_end_3;

	delete add_1;
	delete add_2;
	delete add_3;

	delete test_add_2_1;
	delete test_add_2_2;
	delete test_add_3_1;
	delete test_add_3_2;

	delete add_vec_2_1_beg;
	delete add_vec_2_1_end;
	delete add_vec_2_2_beg;
	delete add_vec_2_2_end;

	delete add_vec_3_1_beg;
	delete add_vec_3_1_end;
	delete add_vec_3_2_beg;
	delete add_vec_3_2_end;

	delete[] add_test_data_2_1_beg;
	delete[] add_test_data_2_1_end;
	delete[] add_test_data_2_2_beg;
	delete[] add_test_data_2_2_end;

	delete[] add_test_data_3_1_beg;
	delete[] add_test_data_3_1_end;
	delete[] add_test_data_3_2_beg;
	delete[] add_test_data_3_2_end;

	delete convex_1_from_intersected;
	delete convex_2_from_intersected;
	delete convex_3_from_intersected;

	delete convex_1;
	delete convex_2;
	delete convex_3;

	delete intersection_1;
	delete intersection_2;
	delete intersection_3;

	delete intersects_comp1_1;
	delete intersects_comp2_1;
	delete intersects_comp3_1;
	delete not_intersects_comp1_1;
	delete not_intersects_comp2_1;
	delete not_intersects_comp3_1;

	delete subset_comp1_1;
	delete subset_comp2_1;
	delete subset_comp3_1;
	delete not_subset_comp1_1;
	delete not_subset_comp2_1;
	delete not_subset_comp3_1;

	delete not_contains_vec1_beg_1;
	delete not_contains_vec1_end_1;
	delete not_contains_vec2_beg_1;
	delete not_contains_vec2_end_1;
	delete not_contains_vec3_beg_1;
	delete not_contains_vec3_end_1;

	delete[] not_contains_data1_beg_1;
	delete[] not_contains_data1_end_1;
	delete[] not_contains_data2_beg_1;
	delete[] not_contains_data2_end_1;
	delete[] not_contains_data3_beg_1;
	delete[] not_contains_data3_end_1;

	delete contains_vec1_beg_1;
	delete contains_vec1_end_1;
	delete contains_vec2_beg_1;
	delete contains_vec2_end_1;
	delete contains_vec3_beg_1;
	delete contains_vec3_end_1;

	delete[] contains_data1_beg_1;
	delete[] contains_data1_end_1;
	delete[] contains_data2_beg_1;
	delete[] contains_data2_end_1;
	delete[] contains_data3_beg_1;
	delete[] contains_data3_end_1;

	delete clone_1;
	delete clone_2;
	delete clone_3;

	delete comp1_1;
	delete comp1_2;
	delete comp2_1;
	delete comp2_2;
	delete comp3_1;
	delete comp3_2;

	delete vec1_beg_1;
	delete vec1_end_1;
	delete vec1_beg_2;
	delete vec1_end_2;
	delete vec2_beg_1;
	delete vec2_end_1;
	delete vec2_beg_2;
	delete vec2_end_2;
	delete vec3_beg_1;
	delete vec3_end_1;
	delete vec3_beg_2;
	delete vec3_end_2;

	delete[] data1_beg_1;
	delete[] data1_end_1;
	delete[] data1_beg_2;
	delete[] data1_end_2;
	delete[] data2_beg_1;
	delete[] data2_end_1;
	delete[] data2_beg_2;
	delete[] data2_end_2;
	delete[] data3_beg_1;
	delete[] data3_end_1;
	delete[] data3_beg_2;
	delete[] data3_end_2;

	return getResult();
}
