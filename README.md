# LAssert
Unary test library in C for Linux

## Notes
Coloration of text only works on Unix like OS.
To enable automatic main, you will need GNU C.

## How to use it
#### Simple macros
The code below show all the simple macros you can use in LAssert :
```c
//#define LASSERT_MANUAL_MAIN /* this macro enable manual main */
//#define LASSERT_SECTION_TIME /* this macro enable time display of each TEST_SECTION */
#include "LAssert.h"

#include <stdio.h>

TEST_SECTION(Section1){
	REQUIRE(1); /* a succesful require */

	TEST_CASE(Case1){
		REQUIRE(0); /* an unsuccesful require resulting in an unsuccesful test case */
		puts("This text will not be printed");
	}

	TEST_CASE(Case2){
		REQUIRE(1);
		puts("This text will be printed");
	}
}

TEST_SECTION(Section2){
	int * p = (int*)malloc(sizeof(int));

	REQUIRE_NOT_NULL(p); /* will fail if p is NULL, not the same error log as require if failed */

	*p = 2;

	free(p);
	p = NULL;

	REQUIRE_NOT_NULL(p);
}

#ifdef LASSERT_MANUAL_MAIN
int main(){
    RUN_SECTION(Section1);
    RUN_SECTION(Section2);

    return 0;
}
#endif
```
Here is what each macro means in case you did not guess :
* TEST_SECTION : define a section of test which can contain test cases
* TEST_CASE : define a test case
* REQUIRE : end the test case or test section in which it was called on fail
* REQUIRE_NOT_NULL : same as REQUIRE but with a different log message on fail
* RUN_SECTION : manually run a test section
#### Advanced macros