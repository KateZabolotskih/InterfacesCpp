#ifndef TEST_H
#define TEST_H

#include <cstddef> // size_t

void setTestName(char const* name);
void outputTest(char const* _case, bool passed, bool resetCount = false);
void reset();
bool getResult();

#endif /* TEST_H */
