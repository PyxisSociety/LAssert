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

TEST_SECTION(equal_test){
    EQ( .3,.300000001);

    TEST_CASE(should_failed){
	EQ(.7,.6);
    }
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
* **TEST_SECTION** : define a section of test which can contain test cases
* **TEST_CASE** : define a test case
* **REQUIRE** : end the test case or test section in which it was called on fail
* **REQUIRE_NOT_NULL** : same as **REQUIRE** but with a different log message on fail
* **RUN_SECTION** : manually run a test section
* **EQ** : test if two numbers are equals (useful for floating point numbers)
#### Advanced macros
The code below show all the other macros you can use in LAssert :
```c
#include "LAssert.h"

TEST_SECTION(random_test){
    RAND_CASE(first_random,tab,3,3,1,10,10,100){
	puts("BEEEEGIN");
	for(unsigned i = 0;i < 3; ++i){
	    printf("%d\n",tab[i]);
	    REQUIRE(tab[i]);
	}
	putchar('\n');
    }

    RAND_CASE(should_failed,tab2,5,5,1){
    }

    RAND_CASE(fail_on_0, tab3, 4, 4, 0, 3){
	for(unsigned i = 0; i < 4; ++i)
	    REQUIRE(tab3[i]);
    }
}

TEST_SECTION(range_test){
    RANGE_CASE(should_succeed,tab,3,1,3,1){
	for(unsigned i = 0; i < 3; ++i)
	    printf("%d ",tab[i]);
	putchar('\n');
    }

    RANGE_CASE(should_failed,tab2,3,1){
    }
}

TEST_SECTION(copy_test){
    unsigned i = 0;

    TEST_CASE(should_succeed){
	COPY(unsigned,i);
	i = 2;
	REQUIRE(i);
    }
    
    /* ONCE has to be put if there are test cases AND code not inside a test_case for the last one not to be done more than once */
    puts("Display more than once");

    ONCE puts("A line displayed only once");

    ONCE {
	puts("A block displayed only once");
    }
    
    TEST_CASE(should_failed){
	REQUIRE(i);
    }
}
```
Here is what each macro means in case you did not guess :
* **RAND_CASE** : special test case which will be run with random numbers, its parameters are<br/>
  * the name of the test case
  * the variable in which we want to store the random values (`int tab[]`) (WARNING : if you use more than one **RAND_CASE** or **RANGE_CASE** in the same section, variable names have to be different)
  * the size of tab
  * an unlimited number of ranges that have the form `begin, end`
* **RANGE_CASE** : special test case which will be run with with all possible combinations of numbers in defined ranges, its parameters are<br/>
  * the name of the test case
  * the variable in which we want to store the random values (`int tab[]`) (WARNING : if you use more than one **RAND_CASE** or **RANGE_CASE** in the same section, variable names have to be different)
  * the size of tab
  * an unlimited number of ranges that have the form `begin, end, step`
* **COPY** : copy a variable of a section in a test case so that the modifications brought by the test case will only be effective in it
* **ONCE** : prevent a code inside a section but outside a test case to be called more than once<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This problem can occure when you mix up test cases and code not inside test cases