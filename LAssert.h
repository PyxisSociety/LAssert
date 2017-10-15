#ifndef _LASSERT_MAIN_KLEVH_
#define _LASSERT_MAIN_KLEVH_

#include <stdio.h>
#include <stdlib.h>

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

/**
 * @brief Function to launch your tests
 * @param f : function to be launch, the test
 * @param test_name : name of the test
 */
void test_case(void (*f)(void), char * test_name);

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

#endif
