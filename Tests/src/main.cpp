#include <iostream>  // cout
#include <iomanip>	 // setw
#include <conio.h>   // getch

#include "../include/vector.h"
#include "../include/set.h"
#include "../include/compact.h"
#include "../include/problem.h"
#include "../include/solver.h"

int main() {
	std::cout << "\n_____________TESTING IVector_____________\n";
	bool testIVectorPassed = testIVector(true);
	std::cout << "\n______________TESTING ISet_______________\n";
	bool testISetPassed = testISet(true);
	std::cout << "\n_____________TESTING ICompact____________\n";
	bool testICompactPassed = testICompact(true);
    std::cout << "\n_____________TESTING IProblem____________\n";
	bool testIProblemPassed = testIProblem(true);
    std::cout << "\n_____________TESTING ISolver_____________\n";
	bool testISolverPassed = testISolver(true);

	std::cout << "\n\n________________RESULTS________________\n\n";
	//std::cout << "ILogger  TEST: " << (testILoggerPassed  ? "PASSED" : "FAILED") << '\n';
	std::cout << "IVector  TEST: " << (testIVectorPassed  ? "PASSED" : "FAILED") << '\n';
	std::cout << "ISet     TEST: " << (testISetPassed     ? "PASSED" : "FAILED") << '\n';
	std::cout << "ICompact TEST: " << (testICompactPassed ? "PASSED" : "FAILED") << '\n';
	std::cout << "IProblem TEST: " << (testIProblemPassed ? "PASSED" : "FAILED") << '\n';
	std::cout << "ISolver  TEST: " << (testISolverPassed  ? "PASSED" : "FAILED") << '\n';

    std::cout << "\nPress any button...\n";
    getch();

	return 0;
}
