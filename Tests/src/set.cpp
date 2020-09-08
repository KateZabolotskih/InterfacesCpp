#include "../include/set.h"
#include "../include/test.h"

#ifndef LIB_IMPORT
#define LIB_IMPORT
#endif

#include "../../Logger/include/ILogger.h"
#include "../../Vector/include/IVector.h"
#include "../../Set/include/ISet.h"

#include <assert.h> // assert
#include <new>		// nothrow
#include <iostream> // cout

// static void printSet(ISet const* s) {
// 	if (s == nullptr)
// 		return;
// 	IVector* vec = nullptr;
// 	std::cout << "set:\n";
// 	for (size_t i = 0; i < s->getSize(); ++i) {
// 		s->get(vec, i);
// 		assert(vec != nullptr);
// 		printVector(vec);
// 		delete vec;
// 	}
// 	std::cout << '\n';
// }

bool testISet(bool useLogger) {
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
		logger->setLogFile("ISet.log");
	}
	
    reset();
	setTestName("ISet");

	size_t dim1 = 3;
	size_t dim2 = 4;

	double* data1 = new(std::nothrow) double[dim1];
	double* data2 = new(std::nothrow) double[dim1];
	double* data3 = new(std::nothrow) double[dim1];
	double* data4 = new(std::nothrow) double[dim2];
	assert(data1 != nullptr);
	assert(data2 != nullptr);
	assert(data3 != nullptr);
	assert(data4 != nullptr);

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

	IVector::Norm norm = IVector::Norm::NORM_2;
	double tolerance = 0.1;

	IVector* vec1 = IVector::createVector(dim1, data1, logger);
	IVector* vec2 = IVector::createVector(dim1, data2, logger);
	IVector* vec3 = IVector::createVector(dim1, data3, logger);
	IVector* vec4 = IVector::createVector(dim2, data4, logger);
	assert(vec1 != nullptr);
	assert(vec2 != nullptr);
	assert(vec3 != nullptr);
	assert(vec4 != nullptr);

	// no need to delete
	IVector* clonevec1 = nullptr;
	IVector* clonevec2 = nullptr;
	IVector* nullvec   = nullptr;
	ISet*    nullset   = nullptr;

	// ISet::createSet
	ISet* set1 = ISet::createSet(logger);
	ISet* set2 = ISet::createSet(logger);
	ISet* set3 = ISet::createSet(logger);
	outputTest("createSet",
		set1 != nullptr &&
		set2 != nullptr &&
		set3 != nullptr,
		true);
	assert(
		set1 != nullptr &&
		set2 != nullptr &&
		set3 != nullptr);  // no need to continue

	// ISet::getDim
	outputTest("getDim",
		set1->getDim() == 0 &&
		set2->getDim() == 0 &&
		set3->getDim() == 0,
		true);

	// ISet::getSize
	outputTest("getSize",
		set1->getSize() == 0 &&
		set2->getSize() == 0 &&
		set3->getSize() == 0,
		true);

	// ISet::insert
	outputTest("insert",
		set1->insert(vec1, norm, tolerance) == ReturnCode::RC_SUCCESS 	&&
		set1->getDim() == dim1 											&&
		set1->getSize() == 1,
		true);

	outputTest("insert",
		set1->insert(vec2, norm, tolerance) == ReturnCode::RC_SUCCESS 	&&
		set1->getDim() == dim1 											&&
		set1->getSize() == 2);

    set1->insert(vec2, norm, tolerance);
	outputTest("insert",
		set1->getDim() == dim1 &&
		set1->getSize() == 2);

	outputTest("insert",
		set2->insert(vec2, norm, tolerance) == ReturnCode::RC_SUCCESS 	&&
		set2->getDim() == dim1 											&&
		set2->getSize() == 1);

	outputTest("insert",
		set2->insert(vec3, norm, tolerance) == ReturnCode::RC_SUCCESS 	&&
		set2->getDim() == dim1 											&&
		set2->getSize() == 2);

	outputTest("insert",
		set1->insert(vec4, norm, tolerance) != ReturnCode::RC_SUCCESS &&
		set1->getDim() == dim1 									      &&
		set1->getSize() == 2);

	//ISet::get
	IVector* vec5 = nullptr;
	bool res;
	outputTest("get",
		set1->get(vec5, 0) == ReturnCode::RC_SUCCESS                                        &&
		IVector::equals(vec1, vec5, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
        res == true,
		true);

	IVector* vec6 = nullptr;
	outputTest("get",
		set1->get(vec6, 1) == ReturnCode::RC_SUCCESS                                        &&
		IVector::equals(vec2, vec6, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
        res == true);

	outputTest("get",
		set1->get(nullvec, 2) != ReturnCode::RC_SUCCESS &&
		nullvec == nullptr);

	//ISet::find
	size_t ind;
	outputTest("find",
		set1->find(vec1, norm, tolerance, ind) == ReturnCode::RC_SUCCESS,
		true);

	outputTest("find",
		set1->find(vec2, norm, tolerance, ind) == ReturnCode::RC_SUCCESS);

	outputTest("find",
		set1->find(vec3, norm, tolerance, ind) != ReturnCode::RC_SUCCESS);

	// ISet::clone
	ISet* set4 = set1->clone();
	ISet* set5 = set2->clone();
	outputTest("clone",
		set4 != nullptr &&
		set5 != nullptr &&
		set4->getDim() == set1->getDim() &&
		set5->getDim() == set2->getDim() &&
		set4->getSize() == set1->getSize() &&
		set5->getSize() == set2->getSize(),
		true);
	assert(
		set4 != nullptr &&
		set5 != nullptr);

	for (size_t i = 0; i < set4->getSize(); ++i) {
		set1->get(clonevec1, i);
		set4->get(clonevec2, i);
		assert(clonevec1 != nullptr);
		assert(clonevec2 != nullptr);
		outputTest("clone",
			IVector::equals(clonevec1, clonevec2, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
            res == true);
		delete clonevec1;
		delete clonevec2;
		clonevec1 = nullptr;
		clonevec2 = nullptr;
	}
	for (size_t i = 0; i < set5->getSize(); ++i) {
		set2->get(clonevec1, i);
		set5->get(clonevec2, i);
		assert(clonevec1 != nullptr);
		assert(clonevec2 != nullptr);
		outputTest("clone",
			IVector::equals(clonevec1, clonevec2, norm, tolerance, res, logger) == ReturnCode::RC_SUCCESS &&
            res == true);
		delete clonevec1;
		delete clonevec2;
		clonevec1 = nullptr;
		clonevec2 = nullptr;
	}

	// ISet::erase (by index)
	nullvec = nullptr;
	outputTest("erase (by index)",
		set4->find(vec1, norm, tolerance, ind) == ReturnCode::RC_SUCCESS 		    &&
		set4->erase(ind) == ReturnCode::RC_SUCCESS 									&&
		set4->find(vec1, norm, tolerance, ind) != ReturnCode::RC_SUCCESS,
		true);

    nullvec = nullptr;
	outputTest("erase (by index)",
		set4->find(vec2, norm, tolerance, ind) == ReturnCode::RC_SUCCESS 		    &&
		set4->erase(ind) == ReturnCode::RC_SUCCESS 									&&
		set4->find(vec2, norm, tolerance, ind) != ReturnCode::RC_SUCCESS);

	// ISet::erase (by sample)
	nullvec = nullptr;
	outputTest("erase (by sample)",
		set5->find(vec2, norm, tolerance, ind) == ReturnCode::RC_SUCCESS &&
		set5->erase(vec2, norm, tolerance) == ReturnCode::RC_SUCCESS     &&
		set5->find(vec2, norm, tolerance, ind) != ReturnCode::RC_SUCCESS &&
		set5->get(nullvec, 1) != ReturnCode::RC_SUCCESS                  &&
		nullvec == nullptr,
		true);

    nullvec = nullptr;
	outputTest("erase (by sample)",
		set5->find(vec3, norm, tolerance, ind) == ReturnCode::RC_SUCCESS &&
		set5->erase(vec3, norm, tolerance) == ReturnCode::RC_SUCCESS 	 &&
		set5->find(vec3, norm, tolerance, ind) != ReturnCode::RC_SUCCESS &&
		set5->get(nullvec, 0) != ReturnCode::RC_SUCCESS					 &&
		nullvec == nullptr);

	// ISet::clear
	assert(set4->insert(vec1, norm, tolerance) == ReturnCode::RC_SUCCESS);
	assert(set4->insert(vec2, norm, tolerance) == ReturnCode::RC_SUCCESS);
	assert(set4->insert(vec3, norm, tolerance) == ReturnCode::RC_SUCCESS);
	set4->clear();
	outputTest("clear",
		set4->find(vec1, norm, tolerance, ind) != ReturnCode::RC_SUCCESS &&
		set4->find(vec2, norm, tolerance, ind) != ReturnCode::RC_SUCCESS &&
		set4->find(vec3, norm, tolerance, ind) != ReturnCode::RC_SUCCESS &&
		set4->getDim() == 0 										     &&
		set4->getSize() == 0,
		true);

	// special ISet with IVector of dim == dim2
	ISet* set6 = ISet::createSet(logger);
	assert(
		set6 != nullptr 												&&
		set6->insert(vec4, norm, tolerance) == ReturnCode::RC_SUCCESS 	&&
		set6->getDim() == dim2 											&&
		set6->getSize() == (size_t)1
		);

	// ISet::_union
	ISet* set7 = ISet::_union(set1, set2, norm, tolerance, logger);
	outputTest("union",
		set7 != nullptr 												 &&
		set7->getDim() == dim1 											 &&
		set7->getSize() == (size_t)3 									 &&
		set7->find(vec1, norm, tolerance, ind) == ReturnCode::RC_SUCCESS &&
		set7->find(vec2, norm, tolerance, ind) == ReturnCode::RC_SUCCESS &&
		set7->find(vec3, norm, tolerance, ind) == ReturnCode::RC_SUCCESS &&
		true);

	nullset = ISet::_union(set7, set6, norm, tolerance, logger);	// record will be added to logfile
	outputTest("union",
		nullset == nullptr);

	// ISet::difference
	ISet* set8 = ISet::difference(set1, set2, norm, tolerance, logger);
	outputTest("difference",
		set8 != nullptr 												 &&
		set8->getDim() == dim1 											 &&
		set8->getSize() == (size_t)1 									 &&
		set8->find(vec1, norm, tolerance, ind) == ReturnCode::RC_SUCCESS &&
		set8->find(vec2, norm, tolerance, ind) != ReturnCode::RC_SUCCESS,
		true);

	nullset = ISet::difference(set8, set6, norm, tolerance, logger);	// record will be added to logfile
	outputTest("difference",
		nullset == nullptr);

	// ISet::intersection
	ISet* set9 = ISet::intersection(set1, set2, norm, tolerance, logger);
	outputTest("intersection",
		set9 != nullptr 												 &&
		set9->getDim() == dim1 											 &&
		set9->getSize() == (size_t)1 									 &&
		set9->find(vec2, norm, tolerance, ind) == ReturnCode::RC_SUCCESS &&
		set9->find(vec1, norm, tolerance, ind) != ReturnCode::RC_SUCCESS &&
		set9->find(vec3, norm, tolerance, ind) != ReturnCode::RC_SUCCESS,
		true);

	nullset = ISet::intersection(set9, set6, norm, tolerance, logger);	  // record will be added to logfile
	outputTest("intersection",
		nullset == nullptr);

	// ISet::symmetricDifference
	ISet* set10 = ISet::symmetricDifference(set1, set2, norm, tolerance, logger);
	outputTest("symmetricDifference",
		set10 != nullptr 												  &&
		set10->getDim() == dim1 										  &&
		set10->getSize() == (size_t)2 									  &&
		set10->find(vec1, norm, tolerance, ind) == ReturnCode::RC_SUCCESS &&
		set10->find(vec2, norm, tolerance, ind) != ReturnCode::RC_SUCCESS &&
		set10->find(vec3, norm, tolerance, ind) == ReturnCode::RC_SUCCESS,
		true);

	nullset = ISet::symmetricDifference(set10, set6, norm, tolerance, logger);	// record will be added to logfile
	outputTest("symmetricDifference",
		nullset == nullptr);

	delete set1;
	delete set2;
	delete set3;
	delete set4;
	delete set5;
	delete set6;
	delete set7;
	delete set8;
	delete set9;
	delete set10;

	delete vec1;
	delete vec2;
	delete vec3;
	delete vec4;
	delete vec5;
	delete vec6;

	delete[] data1;
	delete[] data2;
	delete[] data3;
	delete[] data4;

	if (logger != nullptr) {
		logger->releaseLogger(client);
	}
	if (client != nullptr) {
		delete (int*)client;
	}

	return getResult();
}
