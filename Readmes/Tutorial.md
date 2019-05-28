# Tutorial

Unary test library in C, greatly inspired by [Catch2](https://github.com/catchorg/Catch2).

## Notes

Some functionalities are not available on windows:
* making allocation functions return always `NULL`
* the `PERFORMANCE` macro

## How to use it

Here is a small tutorial devided in few steps:
* [Configuration of this tool](#markdown-header-Configuration) and some remarks
* [Simple macros](#markdown-header-Simple-macros) you can use
* [Advanced macros](#markdown-header-Advanced-macros) you can use
* [Events](#markdown-header-events) that you can define
* [Disabling allocation functions](#markdown-header-Disabling-allocation) to make them return `NULL` (not on Windows)
* [Performance testing](#markdown-header-Performance) (not on Windows)



### <a id="markdown-header-Configuration"></a>Configuration and remarks
Some functionalities can be (de)activated by macros only (that need to be put before including LAssert) such as:
* **LASSERT_MANUAL_MAIN**: providing a self define main
* **LASSERT_CUSTOM_ALLOC**: enabling (de)activation of allocation functions (making them work normally or always return `NULL`)
  
The resulting program can have predefined parameters described bellow. If you used **LASSERT_MANUAL_MAIN**, you can still have this behavior by calling `LASSERT_PARAMETERS_INIT(argc, argv)`. Using `-h` option will show you all available parameters but will prevent the program from running the tests.
  

Default options are: -nt -c -out=consol -epsilon=1e-6
Some other functionalities can be (de)activated by macros _and_ parameters such as:
* __-\[n]t__ or __LASSERT_SECTION_TIME__: (de)activate timer functionality for sections
* __-\[n]c__ or __LASSERT_NO_COLOR__: (de)activate color in input
* __-out=\[option]__ or : set the output format. \[option] can be one of the following:
  - **consol** (default behavior): standard output console with information about all test cases in all section
  - **small** or __LASSERT_MINIMIZED_OUTPUT__: smaller output giving only information about sections and not details about their test cases
  - **mini** or __LASSERT_SMALL_OUTPUT__: (stands for minimized) no details at all, it just give the percentage of succeeded test case in every sections (as a summarized result)
  - **xml** or __LASSERT_XML_OUTPUT__: JUnit like XML is rendered in standard output
* __-epsilon=[value]__, __LASSERT_EPSILON__ or `LASSERT_set_epsilon(yourValue)`: set the accepted difference on floating point number comparison (value must be a strictly positive floating point number)
* __-seed=[value]__, __LASSERT_SEED__ or `LASSERT_init_seed(yourSeed)`: set the seed to initialize random number (the function re initialize the random generator), default value use the `time(NULL)` function.
  
__NOTES:__
* In auto main mode, the program will return the number of section on failure.
* In manual main mode, each call to `RUN_SECTION` returns 1 if the section failed due to an error, 2 if it failed due to a `NULL` pointer (call of `REQUIRE_NOT_NULL`) or 0 if all went well.
* In manual main and minimized (or xml) output mode, you need to call `LASSERT_PRINT_OUTPUT()` to show the result. If you call this function in another mode, it will simply do nothing so you should call it either way.
* In XML output mode, all user call to write things in standard and error outputs are locked. Forcedly unlocking will make this option unusable. 



### <a id="markdown-header-Simple-macros"></a>Simple macros
The code below show all the simple macros you can use in LAssert :
```c
#define LASSERT_MAIN
#include "LAssert.h"

#include <stdio.h>

TEST_SECTION(Section1){
	REQUIRE(1); /* a succesful require */

	TEST_CASE(Case1){
		REQUIRE(0, "error message %d", 5); /* an unsuccesful require resulting in an unsuccesful test case */
		puts("This text will not be printed");
	}

	TEST_CASE("case 2"){
		REQUIRE(1);
		puts("This text will be printed");
	}
}

TEST_SECTION("second section"){
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
* **LASSERT_MAIN**: specify that the current file is the main file. You need one and only one file from all those compiled in the program to have this macro before `LAssert.h` inclusion.
* **TEST_SECTION**: define a section of test which can contain test cases
* **TEST_CASE**: define a test case
* **REQUIRE**: end the test case or test section in which it was called on fail
* **REQUIRE_NOT_NULL**: same as **REQUIRE** but with a different log message on fail
* **RUN_SECTION**: manually run a test section
* **EQ**: test if two numbers are equals (useful for floating point numbers)
* **EQ_EPS**: test if two numbers are equals (useful for floating point numbers) with a user define tolerated error

__NOTES:__
* As you may have noticed, you can add formatted string like printf parameters to REQUIRE, REQUIRE_NOT_NULL and EQ macros. This formatted string will be shown only when the test will fail.
* Sections and cases names can be either an identifier or a string
  
_WARNING:_ On contrary to printf first argument, it has to be a string constant, not a variable (even variable constants are not allowed)

### <a id="markdown-header-Advanced-macros"></a>Advanced macros
The code below show all the other macros you can use in LAssert :
```c
#include LASSERT_MAIN
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

    LASSERT_set_rand_function(myRand);
    RAND_CASE(selfDefineRandFunction, tab4, 1, 5, 0, 1000){
        REQUIRE(!*tab4);
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

TEST_SECTION("values test"){
    int i[] = {1, 2, 3, 4};

    VALUES_CASE("should succeed", int, var, i){
        REQUIRE(var, "failed with value %d\n", var);
    }
    
    i[3] = 0;

    VALUES_CASE("should fail", int, var, i){
        REQUIRE(var, "failed with value %d\n", var);
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

TEST_SECTION(logs){
    INFO("COUCOU %s", "johnny");

    CHECK(0);

    TEST_CASE("warning case"){
        CHECK(0);
        WARNING("be careful");
        REQUIRE(0);
        INFO("should not be printed");
    }

    ERROR("oups");

    CHECK(0);

    REQUIRE(0);
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
* **VALUES_CASE** : special test case which will be run for all values in an array, its parameters are<br/>
  * the name of the test case
  * the type of each items in the array
  * the variable name which will store the value of the current item (it is copied)
  * the array of values (it needs to be a variable of type static array)
* **COPY** : copy a variable of a section in a test case so that the modifications brought by the test case will only be effective in it
* **ONCE** : prevent a code inside a section but outside a test case to be called more than once<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This problem can occure when you mix up test cases and code not inside test cases
* **INFO**, **WARNING** and **ERROR**: Add a message to be printed (as info, warning or error) each time a failure occure in the section / case. If those macros are called in a test case, they are effective only inside, else they are effective everywhere in the section. Two macros help configure this tool:
  * **LASSERT_MAX_INFOS**: maximum number of times those macros can be called in a section (called in a test case count as called in the section in this case, for optimization purposes)
  * **LASSERT_MAX_INFO_LENGTH**: maximum length of the message which can be put in those macros, once it has been formated.

__NOTES:__
* You can set the random function using `LASSERT_set_rand_function`. The function needs to have the following prototype `int (*) (void)`.
* **INFO**, **ERROR** and **WARNING** format the string on call, and not on assertion failure.



### <a id="markdown-header-events"></a>Events

You can use five functions to catch five events and do something accordingly using those functions:
* `void LASSERT_on_section_begin(void (*)(const char * sectionName))`: call each time a section begin
* `void LASSERT_on_section_end(void (*)(const char * sectionName))`: call each time a section ends
* `void LASSERT_on_case_begin(void (*)(const char * caseName))`: call each time a case begin
* `void LASSERT_on_case_end(void (*)(const char * caseName))`: call each time a section ends
* `void LASSERT_on_assertion_failure((void (*)(char isInCase, const char * caseOrSectionName))` calls each time an assertion fails

```c
#define LASSERT_MAIN
#include "../../LAssert.h"

void event_begin_name(const char * name){
    printf("begin %s\n", name);
}
void event_end_name(const char * name){
    printf("end %s\n", name);
}
void event_failure(char isInCase, const char * name){
    printf("Failure in %s --- is in case ? %d\n", name, isInCase);
}
TEST_SECTION("set event init"){
    LASSERT_on_section_begin(event_begin_name);
}
TEST_SECTION("redefined event behavior"){
    LASSERT_on_section_begin(NULL);
    LASSERT_on_section_end(event_end_name);
    LASSERT_on_case_begin(event_begin_name);
    LASSERT_on_case_end(event_end_name);
    LASSERT_on_assertion_failure(event_failure);

    TEST_CASE("test case"){
        REQUIRE(0);
    }

    RAND_CASE("rand case", tab1, 1, 1, 1, 10){
    }

    RANGE_CASE("range case", tab2, 1, 0, 1, 1){
    }

    REQUIRE(0);
}
TEST_SECTION("set event deinit"){
    LASSERT_on_section_end(NULL);
    LASSERT_on_case_begin(NULL);
    LASSERT_on_case_end(NULL);
    LASSERT_on_assertion_failure(NULL);
}
```



### <a id="markdown-header-Disabling-allocation"></a>Disabling allocation

With those tools, you can render allocation functions to return `NULL` whenever they are called. You have to do three modifications for that:
* You need to call a function in your code (see example below)
* You need to define the macro `LASSERT_CUSTOM_ALLOC` before including `LAssert.h`
* You need to link `libLAssert_alloc.so` shared library (built using `make` in the main LAssert folder) and using `-Wl,-rpath,/path/to/folder/containing/libLAssert_alloc.so -L/same/path -lLAssert_alloc -ldl`

```c
#define LASSERT_MAIN
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
#include LASSERT_MAIN
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
