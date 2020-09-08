#include "../include/ISolver.h"
#include "SolverImpl.cpp"

ISolver::~ISolver() {}

extern "C" {
    DECLSPEC void* getBroker() {
        return (void*)BrokerImpl::getInstance();
    }
}