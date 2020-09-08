#include "../include/test.h"
#include <iostream>  // cout
#include <iomanip>	 // setw

// the entire file is little awkward
// because of refactor, so whatever

static char const* g_name = "";
static bool g_passed = true;

void setTestName(char const* name) {
    g_name = name;
}

void outputTest(char const* _case, bool passed, bool resetCount) {
	static size_t number = 0;

	if (!passed) {
		g_passed = false;
	}

	if (resetCount) {
		number = 0;
		std::cout << '\n';
	}

	std::cout
		<< std::setw(7)
		<< g_name
		<< " :: "
		<< std::setw(18)
		<< _case
		<< " "
		<< std::setw(2)
		<< ++number
		<< (passed ? " PASSED " : " FAILED ")
		<< '\n';
}

void reset() {
	g_name = "";
	g_passed = true;
}

bool getResult() {
	return g_passed;
}
