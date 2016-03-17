#include <stdio.h>
#include "CuTest.h"

/* Declare your own sources of test suites here (or include a header). */
CuSuite* CuGetSuite();
CuSuite* CuStringGetSuite();

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	/* Add your own test suites here. */
	CuSuiteConsume(suite, CuGetSuite());
	CuSuiteConsume(suite, CuStringGetSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);

	CuStringDelete(output);
	CuSuiteDelete(suite);
}

int main(void)
{
	RunAllTests();
}
