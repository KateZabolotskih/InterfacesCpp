#include "../include/problem.h"
#include "../include/test.h"

#include "../../Util/Export.h"
#include "../../Util/Import.h"
#include "../../Util/IBroker.h"
#include "../../Util/ReturnCode.h"
#include "../../Logger/include/ILogger.h"
#include "../../Vector/include/IVector.h"
#include "../../Compact/include/ICompact.h"
#include "../../Problem/include/IProblem.h"

#include <assert.h> // assert
#include <new>		// nothrow
#include <iostream> // cout
#include <cstdlib>  // rand
#include <cmath>    // nan, isnan

typedef IBroker* (*GetBrokerFunc)();

bool testIProblem(bool useLogger) {
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
		logger->setLogFile("IProblem.log");
	}

    reset();
    setTestName("IProblem");

    char const* libname = "problem.dll";
	LIBTYPE hDLLIProblem = LIBOPEN(libname);
	if (hDLLIProblem == NULL) {
        std::cout << "Unable to load library " << libname << '\n';
        return false;
    }

    char const* gbfname = "getBroker";
    GetBrokerFunc gbfIProblem = (GetBrokerFunc)LIBFUNC(hDLLIProblem, "getBroker");
    // GetBrokerFunc gbfIProblem = (IBroker*(*)())LIBFUNC(hIProblemDLL, "getBroker");
    if (gbfIProblem == NULL) {
        std::cout << "Unable to get function address " << gbfname << '\n';
        LIBCLOSE(hDLLIProblem);
        return false;
    }

    IBroker* brokerIProblem = reinterpret_cast<IBroker*>((*gbfIProblem)());
    if (brokerIProblem == NULL) {
        std::cout << "Unable to get IBroker object" << '\n';
        LIBCLOSE(hDLLIProblem);
        return false;
    }

    if (!brokerIProblem->canCastTo(IBroker::BrokerType::BT_PROBLEM)) {
        std::cout << "IBroker cannot provide required implementation" << '\n';
        LIBCLOSE(hDLLIProblem);
        return false;
    }

    IProblem* problem = reinterpret_cast<IProblem*>(brokerIProblem->getInterfaceImpl(IBroker::BrokerType::BT_PROBLEM));
    if (problem == nullptr) {
        std::cout << "Unable to get required interface implementation" << '\n';
        LIBCLOSE(hDLLIProblem);
        return false;
    }


    size_t compactDim = problem->getArgsDim();
    outputTest("getArgsDim",
        compactDim != 0,
        true);

    size_t paramsDim = problem->getParamsDim();
    outputTest("getParamsDim",
        compactDim != 0,
        true);

    double* compactBeginData = new(std::nothrow) double[compactDim];
    double* compactEndData   = new(std::nothrow) double[compactDim];
    assert(compactBeginData != nullptr);
    assert(compactEndData   != nullptr);
    for (size_t i = 0; i < compactDim; ++i) {
        compactBeginData[i] = -3.0;
        compactEndData[i]   =  3.0;
    }

    double* paramsData = new(std::nothrow) double[paramsDim];
    assert(paramsData != nullptr);
    for (size_t i = 0; i < paramsDim; ++i) {
        int r = (rand() % 10) * ((rand() % 2) ? 1 : -1);
        paramsData[i] = (double)r;
    }

    double* stepData = new(std::nothrow) double[compactDim];
    assert(stepData != nullptr);
    for (size_t i = 0; i < compactDim; ++i) {
        stepData[i] = 2.5;
    }
    
    double tolerance = 1e-3;
    IVector*  params       = IVector::createVector(paramsDim, paramsData, logger);
    IVector*  compactBegin = IVector::createVector(compactDim, compactBeginData, logger);
    IVector*  compactEnd   = IVector::createVector(compactDim, compactEndData, logger);
    IVector*  step         = IVector::createVector(compactDim, stepData, logger);
    ICompact* compact      = ICompact::createCompact(compactBegin, compactEnd, tolerance, logger);

    assert(params       != nullptr);
    assert(compactBegin != nullptr);
    assert(compactEnd   != nullptr);
    assert(compact      != nullptr);

    // depends on IProblem implementation (!)
    outputTest("isValidCompact",
        problem->isValidCompact(compact) == true,
        true);

    outputTest("setParams",
        problem->setParams(params) == ReturnCode::RC_SUCCESS,
        true);

    ICompact::Iterator* iterator = compact->begin(step);
    assert(iterator != nullptr);

    IVector* point = nullptr;
    ReturnCode rc = ReturnCode::RC_SUCCESS;
    double res = std::nan("1");
    bool firstTest = true;
    for (point = iterator->getPoint(); rc == ReturnCode::RC_SUCCESS; rc = iterator->doStep(), point = iterator->getPoint()) {
        assert(point != nullptr);

        outputTest("objectiveFunction",
            problem->objectiveFunction(res, point) == ReturnCode::RC_SUCCESS &&
            !std::isnan(res),
            firstTest);

        firstTest = false;
        delete point;
        point = nullptr;
    }

    delete stepData;
    delete paramsData;
    delete compactEndData;
    delete compactBeginData;
    delete compact;
    delete step;
    delete compactEnd;
    delete compactBegin;
    delete params;

    LIBCLOSE(hDLLIProblem);

    if (logger != nullptr) {
		logger->releaseLogger(client);
	}
	if (client != nullptr) {
		delete (int*)client;
	}

	return getResult();
}
