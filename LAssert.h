#ifndef _LASSERT_MAIN_KLEVH_
#define _LASSERT_MAIN_KLEVH_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__)
#define NORMAL "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#else
#define NORMAL ""
#define RED ""
#define GREEN ""
#define YELLOW ""
#define BLUE ""
#endif

#define BEGIN_SECTION(NAME)					\
    void _test_##NAME##_session(char * name){			\
	char name_of_test[1024];				\
	printf(BLUE "BEGIN OF SESSION %s\n" NORMAL, name);

#define END_SECTION						\
    _start_test(name_of_test);					\
    printf(BLUE "\nEND OF SESSION %s\n" NORMAL, name);		\
    }

#define TEST_CASE(NAME_OF_TEST)				\
    _start_test(name_of_test);				\
    strcpy(name_of_test,#NAME_OF_TEST);			\
    printf(NORMAL "\nBegin of test %s\n",name_of_test);

#define RUN_SECTION(NAME) _test_##NAME##_session(#NAME)

/**
 * @brief Equivalente of assert.h's assert for this library
 * @param bool : expression that has to be true
 */
#define require(bool) {				\
	if(!(bool))				\
	    _require_failed(#bool);		\
	else					\
	    _require_succeed();			\
    }

/**
 * @brief Test if a pointer is not NULL (not considered an error but stop the tests)
 * @param ptr : pointer to be tested
 */
#define require_not_null(ptr){			\
	if(!ptr)				\
	    _require_not_null_failed(#ptr);	\
    }

/* ------------- FUNCTIONS NOT TO BE CALLED ------------- */
void _require_failed(char * statement);
void _require_succeed(void);
void _require_not_null_failed(char * ptr);
void _test_case(void (*f)(void), char * test_func, char * test_name);
int _start_test(char * s);

#endif
