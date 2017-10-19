#ifndef _LASSERT_MAIN_KLEVH_
#define _LASSERT_MAIN_KLEVH_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__)) && !defined(LASSERT_NO_COLOR)
#include "unistd.h"
#define NORMAL (isatty(1)?"\x1B[0m":"")
#define RED (isatty(1)?"\x1B[31m":"")
#define GREEN (isatty(1)?"\x1B[32m":"")
#define YELLOW (isatty(1)?"\x1B[33m":"")
#define BLUE (isatty(1)?"\x1B[34m":"")
#define CYAN (isatty(1)?"\x1B[36m":"")
#define MAGENTA (isatty(1)?"\x1B[35m":"")
#else
#define NORMAL ""
#define RED ""
#define GREEN ""
#define YELLOW ""
#define BLUE ""
#define CYAN ""
#define MAGENTA ""
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
	printf("\t%sFailed statement case :\n\t\t%s %s\n",RED, statement,NORMAL);
    else
	printf("%sA test failed but statement could not be read (NULL PTR)%s\n",RED,NORMAL);
}
void _REQUIRE_CASE_succeed(void){
    printf("\t%stest passed%s\n",GREEN,NORMAL);
    nb_tests(0);
}
void _REQUIRE_CASE_not_null_failed(char * ptr){
    if(ptr)
	printf("\t%sFailed to allocate a pointer :\n\t\t%s %s\n",YELLOW,ptr,NORMAL);
    else
	printf("%sFailed to allocate a pointer :\n\t%sCouldn't read pointer's name%s\n",YELLOW,RED,NORMAL);
}
int _start_test(char * s,int option,int set_start){
    static int started = 0;
    static char * statement;

    if(option){
	if(started){
	    if(statement){
		printf("\t%s%llu test(s) passed%s\n",GREEN, nb_tests(1),NORMAL);
		printf("%sEnd of test_case %s %s\n",MAGENTA,statement,NORMAL);
	    }
	    statement = s;
	}else{
	    started = set_start;
	    statement = s;
	}
	nb_tests(-1);
    }else
	started = 0;

    return started;
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
    if(option){
	_in_case(1);
	old = option;
    }else{
	--old;
	if(!old)
	    _in_case(0);
    }
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
void _REQUIRE_succeed(char * statement){
    if(statement)
	printf("\n%sSuccesfull statement outside a test case:\n\t%s %s\n",GREEN,statement,NORMAL);
    _succeeded_test_case(1,0);
}
void _REQUIRE_failed(char * statement){
    if(statement)
	printf("\n%sFailed statement outside a test case :\n\t%s %s\n", RED,statement,NORMAL);
    _failed_test_case(1,0);
}
void _REQUIRE_not_null_failed(char * statement, int display){
    if(statement)
	printf("\n%sFailed to allocate outside a test case :\n\t%s %s\n",YELLOW, statement,NORMAL);
    _failed_test_case(1,0);
}
void _generate_tab(int * tab, unsigned size, char * attributes, ... ){
    va_list vl;
    unsigned number_of_parameter = 1;
    int begin, end;
    size_t id = 0;
    
    if(tab){
	while(attributes[id]){
	    if(attributes[id] == ',')
		++number_of_parameter;
	    ++id;
	}

	if(number_of_parameter&1){
	    printf("%sBad parameter in a RAND_CASE :\n\tParameter should be (name of test, name of variable, number of values, number of times, ranges...)\n\tranges is a collection of integer (an even quantity)%s\n",RED,NORMAL);
	    while(_start_running(0));
	}else{
	    va_start(vl,attributes);
	    for(id = 0; id < number_of_parameter/2; ++id){
		begin = va_arg(vl,int);
		end = va_arg(vl,int);
		tab[id] = rand()%(end - begin) + begin;
	    }
	    va_end(vl);
	    for(;id < size; ++id)
		tab[id] = rand()%(end - begin) + begin;
	}
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
	printf("%s------------------------------"			\
	       "------------------------------\n"			\
	       "BEGIN OF SECTION %s %s\n",BLUE,#name,NORMAL);		\
	_succeeded_test_case(0,1);					\
	_start_test(NULL,0,0);						\
	_failed_test_case(0,1);						\
									\
	_test_##name##_lassert(s,&id,log);				\
									\
	if(id == 1)							\
	    _failed_test_case(1,0);					\
	else if(!id)							\
	    _succeeded_test_case(1,0);					\
	_start_test(#name,1,1);						\
	if(!_succeeded_test_case(0,0) && !_failed_test_case(0,0))	\
	    printf("\n%sEMPTY TEST SECTION%s\n",CYAN, NORMAL);		\
	printf("\n%sEND OF SECTION %s %s\n", BLUE, #name, NORMAL);	\
	if(_failed_test_case(0,0))					\
	    printf("%sFailed : %d test_case(s)%s\n",RED,_failed_test_case(0,0),NORMAL); \
	if(_succeeded_test_case(0,0))					\
	    printf("%sSucceeded : %d test_case(s)%s\n",GREEN,_succeeded_test_case(0,0),NORMAL);	\
	printf("%s------------------------------"			\
	       "------------------------------%s\n\n",			\
	       BLUE,NORMAL);						\
    }									\
    void _test_##name##_lassert(char * name_of_test, int * _id_flag, char _log_lassert[512])
    
#define TEST_CASE(NAME_OF_TEST)						\
    if(*_id_flag == 1)							\
	_failed_test_case(1,0);						\
    else if(!*_id_flag)							\
	_succeeded_test_case(1,0);					\
    _start_test(name_of_test,1,1);					\
    strcpy(name_of_test,#NAME_OF_TEST);					\
    printf("\n%sBegin of test_case %s %s\n",MAGENTA,name_of_test,NORMAL); \
    _start_running(1);							\
    _in_case(1);							\
    *_id_flag = 0;							\
    while(_start_running(0))

#define RAND_CASE(NAME_OF_TEST,var_name,nb_of_values,nb_of_time,ranges...) \
    int var_name[nb_of_values] = {0};					\
    if(*_id_flag == 1)							\
	_failed_test_case(1,0);						\
    else if(!*_id_flag)							\
	_succeeded_test_case(1,0);					\
    _start_test(name_of_test,1,1);					\
    strcpy(name_of_test,#NAME_OF_TEST);					\
    printf("\n%sBegin of test_case %s %s\n",MAGENTA,name_of_test,NORMAL); \
    _start_running(nb_of_values);					\
    _in_case(1);							\
    *_id_flag = 0;							\
    while(_generate_tab(var_name,nb_of_values,#ranges,ranges),_start_running(0))

#define REQUIRE(bool){				\
	_start_test(NULL,1,0);			\
	if(!(bool)){				\
	    _REQUIRE_failed(#bool);		\
	    *_id_flag = 1;			\
	    return;				\
	}else					\
	    _REQUIRE_succeed(#bool);		\
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
#define REQUIRE_CASE_NOT_NULL(ptr){			\
	if(!ptr){					\
	    _REQUIRE_CASE_not_null_failed(#ptr);	\
	    _in_case(0);				\
	    break;					\
	}						\
    }

#define REQUIRE_NOT_NULL(ptr){			\
	_start_test(NULL,1,0);			\
	if(!(ptr)){				\
	    _REQUIRE_not_null_failed(#ptr);	\
	    return;				\
	}					\
    }

int main(){
    return 0;
}

#else
/* ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
	printf("\t%sFailed statement :\n\t\t%s%s\n" RED, statement,NORMAL);
    else
	printf("%sA test failed but statement could not be read (NULL PTR)\n%s",RED,NORMAL);
}

void _REQUIRE_succeed(void){
    nb_tests(0);
}

void _REQUIRE_not_null_failed(char * ptr){
    if(ptr)
	printf("\t%sFailed to allocate a pointer :\n\t\t%s%s\n",YELLOW,ptr,NORMAL);
    else
	printf("%sFailed to allocate a pointer :\n\t%sCouldn't read pointer's name%s\n",YELLOW,RED,NORMAL);
}

int _start_test(char * s,int option){
    static int started = 0;

    if(option){
	if(started){
	    printf("\t%s%llu test(s) passed%s\n",GREEN, nb_tests(1),NORMAL);
	    if(s)
		printf("%sEnd of test %s%s\n",MAGENTA,s,NORMAL);
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
	printf("%s----------------------------------------"		\
	       "--------------------\n"					\
	       "BEGIN OF SECTION %s%s\n"				\
	       BLUE, _name_of_function,NORMAL);				\
	char _start_running = 0;					\
	_start_test(NULL,0);

#define END_SECTION							\
    _start_test(name_of_test,1);					\
    printf("\n%sEND OF SECTION %s\n-------------------------"	\
	   "-----------------------------------%s\n\n"			\
	   BLUE, _name_of_function,NORMAL);					\
    }

#define TEST_CASE(NAME_OF_TEST)				\
    _start_test(name_of_test,1);			\
    strcpy(name_of_test,#NAME_OF_TEST);			\
    printf("\n%sBegin of test %s%s\n",MAGENTA,name_of_test,NORMAL);	\
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
