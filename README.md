# LAssert
Unary test library in C for Linux

## Notes

Some functionalities are not available on windows:
* text color in console output
* making allocation functions return always `NULL`

## How to use it

Here is a small tutorial devided in few steps:
* [Configuration of this tool](#markdown-header-Configuration) and some remarks
* [Simple macros](#markdown-header-Simple-macros) you can use
* [Advanced macros](#markdown-header-Advanced-macros) you can use
* [Disabling allocation functions](#markdown-header-Disabling-allocation) to make them return `NULL` (not on Windows)
* [Performance testing](#markdown-header-Performance) (not on Windows)

### <a id="markdown-header-Configuration"></a>Configuration and remarks
Some functionalities can be (de)activated by macros only (that need to be put before including LAssert) such as:
* **LASSERT_MANUAL_MAIN**: providing a self define main
* **LASSERT_CUSTOM_ALLOC**: enabling (de)activation of allocation functions (making them work normally or always return `NULL`)
  
The resulting program can have predefined parameters described bellow. If you used **LASSERT_MANUAL_MAIN**, you can still have this behavior by calling `LASSERT_PARAMETERS_INIT(argc, argv)`. Using `-h` option will show you all available parameters but will prevent the program from running the tests.
  

Default options are: -nt -c -out=consol
Some other functionalities can be (de)activated by macros _and_ parameters such as:
* __-\[n]t__ or __LASSERT_SECTION_TIME__: (de)activate timer functionality for sections
* __-\[n]c__ or __LASSERT_NO_COLOR__: (de)activate color in input (only available for UNIX systems and in consol output)
* __-out=\[option]__ or : set the output format. \[option] can be one of the following:
  - **consol** (default behavior): standard output console with information about all test cases in all section
  - **small** or __LASSERT_MINIMIZED_OUTPUT__: smaller output giving only information about sections and not details about their test cases
  - **mini** or __LASSERT_SMALL_OUTPUT__: (stands for minimized) no details at all, it just give the percentage of succeeded test case in every sections (as a summarized result)
  - **xml** or __LASSERT_XML_OUTPUT__: JUnit like XML is rendered in standard output
* __-epsilon=[value]__, __LASSERT_EPSILON__ or `LASSERT_set_epsilon(yourValue)`: set the accepted difference on floating point number comparison (value must be a strictly positive floating point number)
* __-seed=[value]__, __LASSERT_SEED__ or `LASSERT_init_seed(yourSeed)`: set the seed to initialize random number (the function re initialize the random generator)
  
__NOTES:__
* In auto main mode, the program will return the number of section on failure.
* In manual main mode, each call to `RUN_SECTION` returns 1 if the section failed due to an error, 2 if it failed due to a `NULL` pointer (call of `REQUIRE_NOT_NULL`) or 0 if all went well.
* In manual main and minimized output mode, you need to call `LASSERT_PRINT_OUTPUT()` to show the result. If you call this function in another mode, it will simply do nothing so you should call it either way.
* In XML output mode, all user call to write things in standard and error outputs are locked. Forcedly unlocking will make this option unusable. 

### <a id="markdown-header-Simple-macros"></a>Simple macros
The code below show all the simple macros you can use in LAssert :
```c
#include "LAssert.h"

#include <stdio.h>

TEST_SECTION(Section1){
	REQUIRE(1); /* a succesful require */

	TEST_CASE(Case1){
		REQUIRE(0, "error message %d", 5); /* an unsuccesful require resulting in an unsuccesful test case */
		puts("This text will not be printed");
	}

	TEST_CASE(Case2){
		REQUIRE(1);
		puts("This text will be printed");
	}
}

TEST_SECTION(Section2){
	int * p = (int*)malloc(sizeof(int));

	REQUIRE_NOT_NULL(p, "will be shown only if malloc failed"); /* will fail if p is NULL, not the same error log as require if failed */

	*p = 2;

	free(p);
	p = NULL;

	REQUIRE_NOT_NULL(p);
}

TEST_SECTION(equal_test){
    EQ( .3,.300000001);

    TEST_CASE(should_failed){
	EQ(.7,.6, "it is not possible");
    }

    EQ_EPS(.7, .6, .2);
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
* **TEST_SECTION**: define a section of test which can contain test cases
* **TEST_CASE**: define a test case
* **REQUIRE**: end the test case or test section in which it was called on fail
* **REQUIRE_NOT_NULL**: same as **REQUIRE** but with a different log message on fail
* **RUN_SECTION**: manually run a test section
* **EQ**: test if two numbers are equals (useful for floating point numbers)
* **EQ_EPS**: test if two numbers are equals (useful for floating point numbers) with a user define tolerated error

__NOTE:__ As you may have noticed, you can add formatted string like printf parameters to REQUIRE, REQUIRE_NOT_NULL and EQ macros. This formatted string will be shown only when the test will fail.
  
_WARNING:_ On contrary to printf first argument, it has to be a string constant, not a variable (even variable constants are not allowed)

### <a id="markdown-header-Advanced-macros"></a>Advanced macros
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

### <a id="markdown-header-Disabling-allocation"></a>Disabling allocation

With those tools, you can render allocation functions to return `NULL` whenever they are called. You have to do three modifications for that:
* You need to call a function in your code (see example below)
* You need to define the macro `LASSERT_CUSTOM_ALLOC` before including `LAssert.h`
* You need to link `libLAssert_alloc.so` shared library (built using `make` in the main LAssert folder) and using `-Wl,-rpath,/path/to/folder/containing/libLAssert_alloc.so -L/same/path -lLAssert_alloc -ldl`

```c
#define LASSERT_CUSTOM_ALLOC
#include "LAssert.h"

TEST_SECTION(alloc_disabled){
    LAssert_alloc(1); // lock allocation
    int * i = (int*)malloc(sizeof(*i));
    REQUIRE(!i);

    LAssert_alloc(0); // unlock allocation
    i = (int*)malloc(sizeof(*i));
    REQUIRE_NOT_NULL(i);
    free(i);
}
```
__NOTES:__
* If `LASSERT_CUSTOM_ALLOC` is not defined, you do not need all this set up.
* You can use your own dynamic library to override allocation functions by defining the macro __LASSERT_LOCK_LIBRARY__ with a constant string containing the name of your library.

### <a id="markdown-header-Performance"></a> Performance testing

You can run an algorithm with a timeout (in seconds). If the timeout is reached before the algorithm ends, the test is considered as failing. You can do as follow:
```c
#include "LAssert.h"

TEST_SECTION(perfo_test){
    TEST_CASE(inside_case_ok){
        PERFORMANCE(1){
            PERFO_EXIT;
        }
    }
    
    TEST_CASE(inside_case_ko){
        PERFORMANCE(1){
            while(1);
            PERFO_EXIT;
        }
    }

    PERFORMANCE(1){
        PERFO_EXIT;
    }

    PERFORMANCE(1){
        while(1);
        PERFO_EXIT;
    }
}
```
__NOTES:__
* You _need_ to end the `PERFORMANCE` block with the `PERFO_EXIT` macro.