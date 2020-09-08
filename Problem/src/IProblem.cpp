#include "../include/IProblem.h"
#include "ProblemImpl.cpp"

IProblem::~IProblem() {}

extern "C" {
    DECLSPEC void* getBroker() {
        return (void*)BrokerImpl::getInstance();
    }
}