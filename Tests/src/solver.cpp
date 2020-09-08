#include "../include/solver.h"
#include "../include/test.h"

#include "../../Util/Export.h"
#include "../../Util/Import.h"
#include "../../Util/IBroker.h"
#include "../../Util/ReturnCode.h"
#include "../../Logger/include/ILogger.h"
#include "../../Vector/include/IVector.h"
#include "../../Compact/include/ICompact.h"
#include "../../Problem/include/IProblem.h"
#include "../../Solver/include/ISolver.h"

#include <assert.h> // assert
#include <new>		// nothrow
#include <iostream> // cout
#include <cmath>    // fabs (C++11)

#define MY_CONFIGURATION

typedef IBroker* (*GetBrokerFunc)();

bool testISolver(bool useLogger) {
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
		logger->setLogFile("ISolver.log");
	}

    reset();
    setTestName("ISolver");

    char const* libnameIProblem = "problem.dll";
    char const* libnameISolver  = "solver.dll";
	LIBTYPE hDLLIProblem = LIBOPEN(libnameIProblem);
	LIBTYPE hDLLISolver = LIBOPEN(libnameISolver);
    if (hDLLIProblem == NULL) {
        std::cout << "Unable to load library " << libnameIProblem << '\n';
        return false;
    }
	if (hDLLISolver == NULL) {
        std::cout << "Unable to load library " << libnameISolver << '\n';
        LIBCLOSE(hDLLIProblem);
        return false;
    }

    char const* gbfname = "getBroker";
    GetBrokerFunc gbfIProblem = (GetBrokerFunc)LIBFUNC(hDLLIProblem, "getBroker");
    GetBrokerFunc gbfISolver  = (GetBrokerFunc)LIBFUNC(hDLLISolver,  "getBroker");
    // GetBrokerFunc gbfIProblem = (IBroker*(*)())LIBFUNC(hIProblemDLL, "getBroker");
    // GetBrokerFunc gbfISolver  = (IBroker*(*)())LIBFUNC(hISolverDLL,  "getBroker");
    if (gbfIProblem == NULL) {
        std::cout << "Unable to get function address: " << gbfname << "for IProblem" << '\n';
        LIBCLOSE(hDLLIProblem);
        LIBCLOSE(hDLLISolver);
        return false;
    }
    if (gbfISolver == NULL) {
        std::cout << "Unable to get function address: " << gbfname << "for ISolver" << '\n';
        LIBCLOSE(hDLLIProblem);
        LIBCLOSE(hDLLISolver);
        return false;
    }

    IBroker* brokerIProblem = reinterpret_cast<IBroker*>((*gbfIProblem)());
    if (brokerIProblem == NULL) {
        std::cout << "Unable to get IBroker object for IProblem" << '\n';
        LIBCLOSE(hDLLIProblem);
        LIBCLOSE(hDLLISolver);
        return false;
    }

    IBroker* brokerISolver = reinterpret_cast<IBroker*>((*gbfISolver)());
    if (brokerISolver == NULL) {
        std::cout << "Unable to get IBroker object for ISolver" << '\n';
        LIBCLOSE(hDLLIProblem);
        LIBCLOSE(hDLLISolver);
        return false;
    }


    if (!brokerIProblem->canCastTo(IBroker::BrokerType::BT_PROBLEM)) {
        std::cout << "IBroker cannot provide required implementation for IProblem" << '\n';
        LIBCLOSE(hDLLIProblem);
        LIBCLOSE(hDLLISolver);
        return false;
    }

    if (!brokerISolver->canCastTo(IBroker::BrokerType::BT_SOLVER)) {
        std::cout << "IBroker cannot provide required implementation for ISolver" << '\n';
        LIBCLOSE(hDLLIProblem);
        LIBCLOSE(hDLLISolver);
        return false;
    }

    IProblem* problem = reinterpret_cast<IProblem*>(brokerIProblem->getInterfaceImpl(IBroker::BrokerType::BT_PROBLEM));
    if (problem == nullptr) {
        std::cout << "Unable to get required interface implementation" << '\n';
        LIBCLOSE(hDLLIProblem);
        LIBCLOSE(hDLLISolver);
        return false;
    }

    ISolver* solver = reinterpret_cast<ISolver*>(brokerISolver->getInterfaceImpl(IBroker::BrokerType::BT_SOLVER));
    if (solver == nullptr) {
        std::cout << "Unable to get required interface implementation" << '\n';
        LIBCLOSE(hDLLIProblem);
        LIBCLOSE(hDLLISolver);
        return false;
    }

    size_t compactDim = problem->getArgsDim();
    size_t paramsDim  = problem->getParamsDim();

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

    double epsilon = 0.05;
    double* stepData = new(std::nothrow) double[compactDim];
    assert(stepData != nullptr);
    for (size_t i = 0; i < compactDim; ++i) {
        stepData[i] = epsilon;
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
    assert(problem->isValidCompact(compact) == true);

    ICompact::Iterator* iterator = compact->begin(step);
    assert(iterator != nullptr);

    outputTest("setProblem",
        solver->setProblem(problem) == ReturnCode::RC_SUCCESS,
        true);

    outputTest("setCompact",
        solver->setCompact(compact) == ReturnCode::RC_SUCCESS,
        true);

    outputTest("setParams",
        solver->setParams(step) == ReturnCode::RC_SUCCESS,
        true);

    outputTest("setProblemParams",
        solver->setProblemParams(params) == ReturnCode::RC_SUCCESS,
        true);

    outputTest("solve",
        solver->solve() == ReturnCode::RC_SUCCESS,
        true);

    IVector* solution = nullptr;
    outputTest("getSolution",
        solver->getSolution(solution) == ReturnCode::RC_SUCCESS &&
        solution != nullptr,
        true);

#ifdef MY_CONFIGURATION
    std::cout << '\n' << "comparing results..." << '\n';
    outputTest("solve",
        (std::fabs(solution->getCoord(0) + params->getCoord(1) / params->getCoord(0)) < 2.0 * epsilon) &&
        (std::fabs(solution->getCoord(1) + params->getCoord(3) / params->getCoord(2)) < 2.0 * epsilon));
#endif

    delete solution;
    delete compact;
    delete step;
    delete compactEnd;
    delete compactBegin;
    delete params;
    delete stepData;
    delete paramsData;
    delete compactEndData;
    delete compactBeginData;

    LIBCLOSE(hDLLIProblem);
    LIBCLOSE(hDLLISolver);

    if (logger != nullptr) {
		logger->releaseLogger(client);
	}
	if (client != nullptr) {
		delete (int*)client;
	}

	return getResult();
}
