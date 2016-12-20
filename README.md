# CuTest

CuTest is a minimalist unit testing suite for the C programming language. Use it
to develop and refactor with confidence.

This is a fork of the original CuTest containing additions and fixes for writing
memory leak free tests. The original version is no longer maintained, but can be
found at http://cutest.sourceforge.net/.

## Getting started

To add unit testing to your C code the only files you need are CuTest.c and
CuTest.h. CuTestTest.c and AllTests.c are included to provide a usage example.

After downloading the sources, run your compiler to create an executable called
AllTests.exe. For example, if you are using Windows with the cl.exe compiler you
would type:

    cl.exe AllTests.c CuTest.c CuTestTest.c
    AllTests.exe

This will run all the unit tests associated with CuTest and print the output on
the console. You can replace cl.exe with gcc or your favorite compiler in the
command above.


## Detailed example

Here is a more detailed example. We will work through a simple test first
exercise. The goal is to create a library of string utilities. First, lets write
a function that converts a null-terminated string to all upper case.

Ensure that CuTest.c and CuTest.h are accessible from your project. Next, create
a file called StrUtil.c with these contents:

```c
#include "CuTest.h"

char* StrToUpper(char* str) {
    return str;
}

void TestStrToUpper(CuTest *tc) {
    char* input = strdup("hello world");
    char* actual = StrToUpper(input);
    char* expected = "HELLO WORLD";
    CuAssertStrEquals(tc, expected, actual);
}

CuSuite* StrUtilGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestStrToUpper);
    return suite;
}
```

Create another file called AllTests.c with the following contents. You can use
the existing AllTests.c in this directory as a template if you wish.

```c
#include <stdio.h>
#include "CuTest.h"

CuSuite* StrUtilGetSuite();

void RunAllTests(void) {
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteConsume(suite, StrUtilGetSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);

    CuStringDelete(output);
    CuSuiteDelete(suite);
}

int main(void) {
    RunAllTests();
}
```

Compile AllTests.c, CuTest.c and StrUtil.c and run the resulting executable.
This will print an error because we haven't implemented the StrToUpper function
yet.

```c
char* StrToUpper(char* str) {
    return str;
}
```

Rewrite this as follows:

```c
char* StrToUpper(char* str) {
    char* p;
    for (p = str ; *p ; ++p) *p = toupper(*p);
    return str;
}
```

Recompile and run the tests again. The test should pass this time.


## What to do next

At this point you might want to write more tests for the `StrToUpper` function.
Here are some ideas:

Test                         | Input
---------------------------- | --------------
`TestStrToUpper_EmptyString` |  ""
`TestStrToUpper_UpperCase`   |  "HELLO WORLD"
`TestStrToUpper_MixedCase`   |  "HELLO world"
`TestStrToUpper_Numbers`     |  "1234 hello"

As you write each one of these tests, add them to the `StrUtilGetSuite`
function. Later as you add more features and write tests for them, be sure to
include those in `StrUtilGetSuite` also.

Over time we might create another file called FunkyStuff.c containing other
functions unrelated to StrUtil. To add tests for our funky stuff, we follow the
same pattern. First create a `FunkyStuffGetSuite` function in FunkyStuff.c and
then add `FunkyStuffGetSuite` to AllTests.c.

## The bigger picture

Each individual test corresponds to a CuTest. These are grouped to form a
CuSuite. CuSuites can hold CuTests or other CuSuites. AllTests.c collects all
the CuSuites in the program into a single CuSuite which it then runs as a single
CuSuite.

The project is open source so feel free to take a peek under the hood at the
CuTest.c file to see how it works. CuTestTest.c contains tests for CuTest.c. So
CuTest tests itself.

Since AllTests.c has a main() you will need to exclude this when you are
building your product. Here is a nicer way to do this if you want to avoid
messing with multiple builds. Remove the main() in AllTests.c. Note that it just
calls RunAllTests(). Instead we'll call this directly from the main program.

Now in the main() of the actual program check to see if the command line option
"--test" was passed. If it was then I call RunAllTests() from AllTests.c.
Otherwise run the real program.

Shipping the tests with the code can be useful. If you customers complain about
a problem you can ask them to run the unit tests and send you the output. This
can help you to quickly isolate the piece of your system that is malfunctioning
in the customer's environment.

CuTest offers a rich set of CuAssert functions. Here is a list:

```c
void CuAssert(CuTest* tc, char* message, int condition);
void CuAssertTrue(CuTest* tc, int condition);
void CuAssertStrEquals(CuTest* tc, char* expected, char* actual);
void CuAssertIntEquals(CuTest* tc, int expected, int actual);
void CuAssertPtrEquals(CuTest* tc, void* expected, void* actual);
void CuAssertPtrNotNull(CuTest* tc, void* pointer);
```

The project is open source and so you can add other more powerful asserts to
make your tests easier to write and more concise. Please feel free to send me
changes you make so that I can incorporate them into future releases.


## Automating test suite generation

make-tests.sh will grep through all the .c files in the current
directory and generate the code to run all the tests contained in
them. Using this script you don't have to worry about writing
AllTests.c or dealing with any of the other suite code.

## Credits

Asim Jalis is the original creator and maintainer of CuTest. Thanks to the
following people for contributing patches!

- [02.23.2003] Dave Glowacki <dglo@hyde.ssec.wisc.edu>
- [04.17.2009] Tobias Lippert <herrmarder@googlemail.com>
- [11.13.2009] Eli Bendersky <eliben@gmail.com>
- [12.14.2009] Andrew Brown <abrown@datasci.com>
