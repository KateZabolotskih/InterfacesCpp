#include "../include/ILogger.h"
#include "LoggerImpl.cpp"

ILogger::~ILogger() {}

ILogger* ILogger::createLogger(void* client) {
	return LoggerImpl::getLogger(client);
}
