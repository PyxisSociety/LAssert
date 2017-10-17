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

#if defined(__GNUC__) && !defined(LASSERT_MANUAL_MAIN)
/* -----------------------------------------------------------
 *            GNU and auto main
 */
unsigned long long nb_tests(int i){
    static unsigned long long count = 0;

    if(!i)
	++count;
    else if(i < 0)
	count = 0;

    return count;
}
void _REQUIRE_CASE_failed(char * statement){
    if(statement)
	printf(RED "\tFailed statement case :\n\t\t%s\n" NORMAL, statement);
    else
	puts(RED "A test failed but statement could not be read (NULL PTR)");
}
void _REQUIRE_CASE_succeed(void){
    puts(GREEN "\ttest passed" NORMAL);
    nb_tests(0);
}
void _REQUIRE_not_null_failed(char * ptr){
    if(ptr)
	printf(YELLOW "\tFailed to allocate a pointer :\n\t\t%s\n" NORMAL,ptr);
    else
	puts(YELLOW "Failed to allocate a pointer :\n\t" RED "Couldn't read pointer's name" NORMAL);
}
int _start_test(char * s,int option){
    static int started = 0;

    if(option){
	if(started){
	    printf(GREEN "\t%llu test(s) passed\n" NORMAL, nb_tests(1));
	    if(s)
		printf(NORMAL "End of test %s\n",s);
	}else
	    started = 1;
	nb_tests(-1);
    }else
	started = 0;
}
int _in_case(int option){
    static int is_in = 0;
    int i = is_in;
    if(option == -1)
	return is_in;
    is_in = option;
    return i;
}
int _start_running(int option){
    static int old = 0;
    int res = old;
    _in_case(old);
    old = option;
    return res;
}
int _failed_test_case(int option, int reset){
    static int count = 0;
    if(reset)
	count = option;
    else
	count += option;
    return count;
}
int _succeeded_test_case(int option, int reset){
    static int count = 0;
    if(reset)
	count = option;
    else
	count += option;
    return count;
}
void _REQUIRE_succeed(char * statement, int display){
    static char * s = NULL;

    if(display){
	if(s)
	    printf("\n" GREEN "Succesfull statement outside a test case:\n\t%s" NORMAL "\n",s);
    }else{
	_succeeded_test_case(1,0);
	s = statement;
    }
}
void _REQUIRE_failed(char * statement, int display){
    static char * s = NULL;

    if(display){
	if(s)
	    printf(RED "\nFailed statement outside a test case :\n\t%s\n" NORMAL, s);
    }else{
	_failed_test_case(1,0);
	s = statement;
    }
}
#define TEST_SECTION(name)						\
    void _test_##name##_lassert( char * name_of_test,int *, char[512]);	\
    void _call_test_##name##_lassert(void)				\
	__attribute__((constructor));					\
    void _call_test_##name##_lassert(void){				\
	char s[512] = {0};						\
	char log[512];							\
	int id = -1;							\
	puts(BLUE "------------------------------"			\
	     "------------------------------\n"				\
	     "BEGIN OF SECTION " #name NORMAL);				\
	_succeeded_test_case(0,1);					\
	_failed_test_case(0,1);						\
	_test_##name##_lassert(s,&id,log);				\
	if(id == 1)							\
	    _failed_test_case(1,0);					\
	else if(!id)							\
	    _succeeded_test_case(1,0);					\
	_start_test(#name,1);						\
	_REQUIRE_succeed(NULL,1);					\
	_REQUIRE_failed(NULL,1);					\
	puts(BLUE "\nEND OF SECTION " #name NORMAL);			\
	printf(RED "Failed : %d test_case(s)\n"				\
	       GREEN "Succeeded : %d test_case(s)\n" NORMAL,		\
	       _failed_test_case(0,0), _succeeded_test_case(0,0));	\
	puts(BLUE "------------------------------"			\
	     "------------------------------\n"				\
	     NORMAL);							\
    }									\
    void _test_##name##_lassert(char * name_of_test, int * _id_flag, char _log_lassert[512])

#define TEST_CASE(NAME_OF_TEST)				\
    if(*_id_flag == 1)					\
	_failed_test_case(1,0);				\
    else if(!*_id_flag)					\
	_succeeded_test_case(1,0);			\
    _start_test(name_of_test,1);			\
    strcpy(name_of_test,#NAME_OF_TEST);			\
    printf(NORMAL "\nBegin of test %s\n",name_of_test);	\
    _start_running(1);					\
    _in_case(1);					\
    *_id_flag = 0;					\
    while(_start_running(0))
/**
 * @brief Equivalente of assert.h's assert for this library
 * @param bool : expression that has to be true
 */
#define REQUIRE(bool){				\
	if(!(bool)){				\
	    _REQUIRE_failed(#bool,0);		\
	    *_id_flag = 1;			\
	    return;				\
	}else					\
	    _REQUIRE_succeed(#bool,0);		\
    }


#define REQUIRE_CASE(bool) {			\
	if(!(bool)){				\
	    _REQUIRE_CASE_failed(#bool);	\
	    *_id_flag = 1;			\
	    _in_case(0);			\
	    continue;				\
	}else					\
	    _REQUIRE_CASE_succeed();		\
    }

/**
 * @brief Test if a pointer is not NULL (not considered an error but stop the tests)
 * @param ptr : pointer to be tested
 */
#define REQUIRE_NOT_NULL(ptr){					\
	if(!ptr){						\
	    _REQUIRE_not_null_failed(#ptr);			\
	    if(_in_case(-1)){					\
		_in_case(0);					\
		break;						\
	    }else						\
		return;						\
	    break;						\
	}							\
    }

int main(){
    return 0;
}

#else
/* -----------------------------------------------------------
 *            none GNU or manual main
 */

unsigned long long nb_tests(int i){
    static unsigned long long count = 0;

    if(!i)
	++count;
    else if(i < 0)
	count = 0;

    return count;
}

/* --------- FUNCTIONS NOT TO BE CALLED DIRECTLY BY USER --------- */
void _REQUIRE_failed(char * statement){
    if(statement)
	printf(RED "\tFailed statement :\n\t\t%s\n" NORMAL, statement);
    else
	puts(RED "A test failed but statement could not be read (NULL PTR)");
}

void _REQUIRE_succeed(void){
    nb_tests(0);
}

void _REQUIRE_not_null_failed(char * ptr){
    if(ptr)
	printf(YELLOW "\tFailed to allocate a pointer :\n\t\t%s\n" NORMAL,ptr);
    else
	puts(YELLOW "Failed to allocate a pointer :\n\t" RED "Couldn't read pointer's name" NORMAL);
}

int _start_test(char * s,int option){
    static int started = 0;

    if(option){
	if(started){
	    printf(GREEN "\t%llu test(s) passed\n" NORMAL, nb_tests(1));
	    if(s)
		printf(NORMAL "End of test %s\n",s);
	}else
	    started = 1;
	nb_tests(-1);
    }else
	started = 0;
}

int _start_running(int option){
    static int i = 0;
    int res = i;
    i = option;

    return res;
}

#define BEGIN_SECTION(NAME)						\
    void _test_##NAME##_session(char * _name_of_function){		\
	char name_of_test[1024];					\
	printf(BLUE							\
	       "----------------------------------------"		\
	       "--------------------\n"					\
	       "BEGIN OF SECTION %s\n"					\
	       NORMAL, _name_of_function);				\
	char _start_running = 0;					\
	_start_test(NULL,0);

#define END_SECTION							\
    _start_test(name_of_test,1);					\
    printf(BLUE "\nEND OF SECTION %s\n-------------------------"	\
	   "-----------------------------------\n\n"			\
	   NORMAL, _name_of_function);					\
    }

#define TEST_CASE(NAME_OF_TEST)				\
    _start_test(name_of_test,1);			\
    strcpy(name_of_test,#NAME_OF_TEST);			\
    printf(NORMAL "\nBegin of test %s\n",name_of_test);	\
    _start_running = 1;					\
    while(_start_running--)

#define RUN_SECTION(NAME) _test_##NAME##_session(#NAME)

/**
 * @brief Equivalente of assert.h's assert for this library
 * @param bool : expression that has to be true
 */
#define REQUIRE(bool) {						\
	if(!(bool)){						\
	    _REQUIRE_failed(#bool);				\
	    break;						\
	}else							\
	    _REQUIRE_succeed();					\
    }

/**
 * @brief Test if a pointer is not NULL (not considered an error but stop the tests)
 * @param ptr : pointer to be tested
 */
#define REQUIRE_NOT_NULL(ptr){					\
	if(!ptr){						\
	    _REQUIRE_not_null_failed(#ptr);			\
	    break;						\
	}							\
    }
#endif
#endif
