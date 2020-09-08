#include "../include/ILogger.h"
#include <iostream> // cerr
#include <new>		// nothrow
#include <vector>   // vector
//#include <time.h>	// time_t, ctime, ctime_s
#include <ctime>    // time_t, ctime, ctime_s
#include <stdio.h>	// FILE, fopen, fopen_s
#include <stdlib.h>

// semicolon isn't necessary
#define INNER_LOG(message) std::cerr << "ERROR: " << __FUNCTION__ << ": " << (message) << '\n';

namespace {
	/* declaration */
	class LoggerImpl : public ILogger {
	private:
		static char const* LOGGER_IMPL_NO_MEM;
		static char const* LOGGER_IMPL_DESTROY_C;
		static char const* LOGGER_IMPL_DESTROY_I;
		static char const* LOGGER_IMPL_LOG_I;
		static char const* LOGGER_IMPL_NULL_PTR;
		static char const* LOGGER_IMPL_SPEC_FILE;
		static char const* LOGGER_IMPL_OPEN_FILE;
		//static char const* LOGGER_IMPL_GET_TIME;

	protected:
		static LoggerImpl* instance;

		std::vector<void*> m_clients;
		FILE* m_logfile;
		LoggerImpl();

	public:
        static ILogger* getLogger(void* client);
		~LoggerImpl()                                        override;
		void releaseLogger(void* client)                     override;
		void log(char const* message, ReturnCode returnCode) override;
		ReturnCode setLogFile(char const* logFileName)       override;
	};
}

/* implementation */
static char const* RC_MESSAGES[(size_t)ReturnCode::RC_UNKNOWN + 1] = {
    "No error",
 	"Not enough memory to allocate data",
 	"Null pointer passed as parameter",
 	"Zero dimension operand identified",
 	"Attempt to operate with instances of incompatible dimensions",
 	"Nan value identified",
 	"Attempt to access data out of bounds",
 	"Unable open file",
 	"Element not found",
 	"Invalid parameters passed",
 	"Initialization of parameters required",
 	"Unknown error"
 };

LoggerImpl* LoggerImpl::instance = nullptr;

/* errors caused by misuse of logger, written to std::cerr */
char const* LoggerImpl::LOGGER_IMPL_NO_MEM	  = "Not enough memory to allocate new LoggerImpl";
char const* LoggerImpl::LOGGER_IMPL_DESTROY_C = "Attempt to destroy LoggerImpl for unknown client";
char const* LoggerImpl::LOGGER_IMPL_DESTROY_I = "Attempt to destroy not instantiated LoggerImpl";
char const* LoggerImpl::LOGGER_IMPL_LOG_I     = "Attempt to log without instantiation LoggerImpl";
char const* LoggerImpl::LOGGER_IMPL_NULL_PTR  = "Nullptr passed as parameter";
char const* LoggerImpl::LOGGER_IMPL_SPEC_FILE = "Log file not specified";
char const* LoggerImpl::LOGGER_IMPL_OPEN_FILE = "Unable to open specified log file";
//char const* LoggerImpl::LOGGER_IMPL_GET_TIME  = "Unable to get system time";

ILogger* LoggerImpl::getLogger(void* client) {
    if (client == nullptr) {
        INNER_LOG(LOGGER_IMPL_NULL_PTR);
        return nullptr;
    }
	if (instance == nullptr) {
		instance = new(std::nothrow) LoggerImpl();
		if (instance == nullptr) {
			INNER_LOG(LOGGER_IMPL_NO_MEM);
			return nullptr;
		}
	}
	for (auto it = instance->m_clients.begin(); it != instance->m_clients.end(); ++it) {
		if (*it == client)
			return instance;
	}
	instance->m_clients.push_back(client);
	return instance;
}

LoggerImpl::LoggerImpl() :
	m_logfile(NULL) {}

LoggerImpl::~LoggerImpl() {
	if (m_logfile != NULL) {
		fflush(m_logfile);
		fclose(m_logfile);
		m_logfile = NULL;
	}
}

void LoggerImpl::releaseLogger(void* client) {
    if (client == nullptr) {
        INNER_LOG(LOGGER_IMPL_NULL_PTR);
        return;
    }
	if (instance == nullptr) {
		INNER_LOG(LOGGER_IMPL_DESTROY_I);
		return;
	}
	for (auto it = instance->m_clients.begin(); it != instance->m_clients.end(); ++it) {
		if (*it == client) {
			instance->m_clients.erase(it);
			if (instance->m_clients.empty() && instance != nullptr) {
				delete instance;
				instance = nullptr;
			}
			return;
		}
	}
	INNER_LOG(LOGGER_IMPL_DESTROY_C);
}

void LoggerImpl::log(char const* message, ReturnCode returnCode) {
	if (instance == nullptr) {
		INNER_LOG(LOGGER_IMPL_LOG_I);
		return;
	}
	if (message == NULL) {
		INNER_LOG(LOGGER_IMPL_NULL_PTR);
		return;
	}
	if (m_logfile == NULL) {
		INNER_LOG(LOGGER_IMPL_SPEC_FILE);
		return;
	}
	time_t _time = time(nullptr);
	fprintf(m_logfile, "%sERROR: %s: %s\n\n", std::ctime(&_time), message, RC_MESSAGES[(size_t)returnCode]);
    //  char buff[26];
    //	if (ctime_s(buff, sizeof(buff), &_time) != 0) {
    //		INNER_LOG(LOGGER_IMPL_GET_TIME)
    //		fprintf(m_logfile, "ERROR: %s: %s\n\n", message, RC_MESSAGES[(size_t)returnCode]);
    //		return;
    //	}
    //	fprintf(m_logfile, "%sERROR: %s: %s\n\n", buff, message, RC_MESSAGES[(size_t)returnCode]);
}

ReturnCode LoggerImpl::setLogFile(char const* logFileName) {
	if (m_logfile != NULL) {
		fflush(m_logfile);
		fclose(m_logfile);
		m_logfile = NULL;
	}
	if (logFileName == NULL) {
		INNER_LOG(LOGGER_IMPL_NULL_PTR);
		return ReturnCode::RC_NULL_PTR;
	}
	//fopen_s(&m_logfile, logFileName, "a");
	m_logfile = fopen(logFileName, "a");
	if (m_logfile == NULL) {
		INNER_LOG(LOGGER_IMPL_OPEN_FILE);
		return ReturnCode::RC_OPEN_FILE;
	}
	return ReturnCode::RC_SUCCESS;
}
