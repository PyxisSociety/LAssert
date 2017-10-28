#ifndef _LASSERT_MAIN_KLEVH_
#define _LASSERT_MAIN_KLEVH_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#if (defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__)) && !defined(LASSERT_NO_COLOR)
#  include <unistd.h>
#  include <sys/time.h>
#  define NORMAL (isatty(1)?"\x1B[0m":"")
#  define RED (isatty(1)?"\x1B[31m":"")
#  define GREEN (isatty(1)?"\x1B[32m":"")
#  define YELLOW (isatty(1)?"\x1B[33m":"")
#  define BLUE (isatty(1)?"\x1B[34m":"")
#  define CYAN (isatty(1)?"\x1B[36m":"")
#  define MAGENTA (isatty(1)?"\x1B[35m":"")
#  define TIME_LASSERT(start)						\
    long start = 0;							\
    struct timeval _timecheck_lassert;					\
    gettimeofday(&_timecheck_lassert, NULL);				\
    start = (long)_timecheck_lassert.tv_sec * 1000 + (long)_timecheck_lassert.tv_usec / 1000
#  define INTERVAL_TIME_LASSERT(start,end) (double)((end) - (start)) / 1000
#else
#  include <windows.h>
#  define NORMAL ""
#  define RED ""
#  define GREEN ""
#  define YELLOW ""
#  define BLUE ""
#  define CYAN ""
#  define MAGENTA ""
#  define TIME_LASSERT(start)				\
    LARGE_INTEGER _frequency_lassert, start;		\
    QueryPerformanceFrequency(&_frequency_lassert);	\
    QueryPerformanceFrequency(&start)
#  define INTERVAL_TIME_LASSERT(start,end) (long double)(end.QuadPart - start.QuadPart)/_frequency_lassert.QuadPart
#endif

unsigned long long nb_tests(int i){
    static unsigned long long count = 0;

    if(!i)
	++count;
    else if(i < 0)
	count = 0;

    return count;
}
void _init_rand_lassert(void){
    static int not_init = 1;
    if(not_init){
	not_init = 0;
	srand(time(NULL));
    }
}
void _REQUIRE_CASE_failed(char * statement){
    if(statement)
	printf("\t%sFailed statement case :\n\t\t%s %s\n",RED, statement,NORMAL);
    else
	printf("%sA test failed but statement could not be read (NULL PTR)%s\n",RED,NORMAL);
}
void _REQUIRE_CASE_succeed(void){
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
    if(option != -1)
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
	if(old)
	    --old;
	if(!res)
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
int _not_null_failed_test_case(int option, int reset){
    static int count = 0;
    if(reset)
	count = option;
    else
	count += option;
    return count;
}
void _REQUIRE_succeed(char * statement){
    _succeeded_test_case(1,0);
}
void _REQUIRE_failed(char * statement){
    if(statement)
	printf("\n%sFailed statement outside a test case :\n\t%s %s\n", RED,statement,NORMAL);
    _failed_test_case(1,0);
}
void _REQUIRE_not_null_failed(char * statement){
    if(statement)
	printf("\n%sFailed to allocate outside a test case :\n\t%s %s\n",YELLOW, statement,NORMAL);
    _failed_test_case(1,0);
}
void _generate_tab(char * name_of_case,int * _id_flag,int * begin,int * end, unsigned size, char * attributes, ... ){
    va_list vl;
    unsigned number_of_parameter = 1;
    size_t id = 0,j;
    
    if(begin && end){
	_init_rand_lassert();
	while(attributes[id]){
	    if(attributes[id] == ',')
		++number_of_parameter;
	    ++id;
	}

	if(number_of_parameter&1){
	    printf("\n%s%s test_case :%s\n",MAGENTA,name_of_case,NORMAL);
	    printf("\t%s%llu test(s) passed%s\n",GREEN, nb_tests(1),NORMAL);
	    printf("\t%sBad parameter in a RAND_CASE :\n\t\t%s\n\t\tSize of list should be even%s\n",RED,attributes,NORMAL);
	    while(_start_running(0));
	    *_id_flag = 1;
	}else{
	    va_start(vl,attributes);
	    for(id = 0; id < number_of_parameter/2; ++id){
		begin[id] = va_arg(vl,int);
		end[id] = va_arg(vl,int);
	    }
	    va_end(vl);
	    for(j = id; j < size; ++j){
		begin[j] = begin[id - 1];
		end[j] = end[id - 1];
	    }
	}
    }
}
void _next_rand_tab(int * tab, int * begin, int * end, int size){
    for(unsigned i = 0; i < size; ++i){
	if(end[i] != begin[i])
	    tab[i] = rand()%(end[i] - begin[i]) + begin[i];
	else
	    tab[i] = end[i];
    }
}
void _generate_range(char * name_of_case,int * _id_flag,int * tab,int * begin, int * end,int * step, unsigned size, char * attributes, ...){
    va_list vl;
    unsigned number_of_parameter = 1;
    size_t id = 0;
    size_t j;
    
    if(begin && end && step && _id_flag && tab){
	while(attributes[id]){
	    if(attributes[id] == ',')
		++number_of_parameter;
	    ++id;
	}

	if(number_of_parameter%3){
	    printf("\n%s%s test_case :%s\n",MAGENTA,name_of_case,NORMAL);
	    printf("\t%s%llu test(s) passed%s\n",GREEN, nb_tests(1),NORMAL);
	    printf("\t%sBad parameter in a RANGE_CASE :\n\t\t%s\n\t\tSize of list should be a multiple of 3%s\n",RED,attributes,NORMAL);
	    *_id_flag = 1;
	}else{
	    va_start(vl,attributes);
	    for(id = 0; id < number_of_parameter/3; ++id){
		begin[id] = va_arg(vl,int);
		tab[id] = begin[id];
		end[id] = va_arg(vl,int);
		step[id] = va_arg(vl,int);
	    }
	    va_end(vl);
	    j = id - 1;
	    for(;id < size; ++id){
		begin[id] = begin[j];
		tab[id] = begin[id];
		end[id] = end[j];
		step[id] = step[j];
	    }
	    tab[size - 1] -= step[size - 1];
	}
    }
}
int _next_range(int * tab, int * begin, int * end, int * step, size_t size){
    int rest     = 1;
    long long id = (long long)(size - 1);

    do{
	tab[id] += step[id];
	if(tab[id] >= end[id]){
	    tab[id] = begin[id];
	    --id;
	}else
	    rest = 0;
    }while(rest && id >= 0);

    if(id < 0)
	_in_case(0);
    
    return id >= 0;
}
double time_used(void){
#ifdef LASSERT_EXEC_TIME
    TIME_LASSERT(start);
    return (double)start;
#else
    return 0.;
#endif
}
int using_time_asked(void){
#ifdef LASSERT_EXEC_TIME
    return 1;
#else
    return 0;
#endif
}

#define EPSILON_LASSERT 1e-6
    
#define COPY(type,var) type var = var
    
#define TEST_CASE(NAME_OF_TEST)				\
    if(*_old_flag < __LINE__){				\
	_tab_lassert = NULL;				\
	if(*_id_flag == 1)				\
	    _failed_test_case(1,0);			\
	else if(!*_id_flag)				\
	    _succeeded_test_case(1,0);			\
	else if(*_id_flag == 2)				\
	    _not_null_failed_test_case(1,0);		\
	_start_test(name_of_test,1,1);			\
	strcpy(name_of_test,#NAME_OF_TEST);		\
	_start_running(1);				\
	_in_case(1);					\
	*_id_flag = 0;					\
    }							\
    while(*_old_flag < __LINE__ && _start_running(0))

#define RAND_CASE(NAME_OF_TEST,var_name,nb_of_values,nb_of_time,ranges...) \
    int var_name[nb_of_values] = {0};					\
    int var_name##begin[nb_of_values] = {0};				\
    int var_name##end[nb_of_values] = {0};				\
    if(*_old_flag < __LINE__){						\
	if(*_id_flag == 1)						\
	    _failed_test_case(1,0);					\
	else if(!*_id_flag)						\
	    _succeeded_test_case(1,0);					\
	else if(*_id_flag == 2)						\
	    _not_null_failed_test_case(1,0);				\
	_start_test(name_of_test,1,1);					\
	_size_of_tab = nb_of_values;					\
	_tab_lassert = var_name;					\
	strcpy(name_of_test,#NAME_OF_TEST);				\
	_start_running(nb_of_values);					\
	_in_case(!*_id_flag);						\
	*_id_flag = 0;							\
	_generate_tab(#NAME_OF_TEST,_id_flag,var_name##begin,var_name##end,nb_of_values,#ranges,ranges); \
    }									\
    while(*_old_flag < __LINE__ && (_next_rand_tab(var_name,var_name##begin,var_name##end,nb_of_values),_start_running(0)) && !*_id_flag)

#define RANGE_CASE(NAME_OF_TEST,var_name,nb_of_values,ranges...)	\
    int var_name[nb_of_values] = {0};					\
    int var_name##_begin[nb_of_values] = {0};				\
    int var_name##_end[nb_of_values] = {0};				\
    int var_name##_step[nb_of_values] = {0};				\
    if(*_old_flag < __LINE__){						\
	_size_of_tab = nb_of_values;					\
	_tab_lassert = var_name;					\
	if(*_id_flag == 1)						\
	    _failed_test_case(1,0);					\
	else if(!*_id_flag)						\
	    _succeeded_test_case(1,0);					\
	else if(*_id_flag == 2)						\
	    _not_null_failed_test_case(1,0);				\
	_start_test(name_of_test,1,1);					\
	strcpy(name_of_test,#NAME_OF_TEST);				\
	*_id_flag = 0;							\
	_generate_range(#NAME_OF_TEST,_id_flag,var_name,var_name##_begin,var_name##_end,var_name##_step,nb_of_values,#ranges,ranges); \
	_in_case(1);							\
    }									\
    while(*_old_flag < __LINE__ && _next_range(var_name,var_name##_begin,var_name##_end,var_name##_step,nb_of_values) && !*_id_flag)

#define REQUIRE(bool){							\
	if(*_old_flag < __LINE__){					\
	    if(!_in_case(-1))						\
		_start_test(NULL,1,0);					\
	    if(!(bool)){						\
		if(_in_case(-1)){					\
		    *_has_to_quit = __LINE__;				\
		    printf("\n%s%s test_case :%s\n",MAGENTA,name_of_test,NORMAL); \
		    printf("\t%s%llu test(s) passed%s\n",GREEN, nb_tests(1),NORMAL); \
		    _REQUIRE_CASE_failed(#bool);			\
		    if(_tab_lassert){					\
			printf("\t%sFailed on this sequence :\n\t\t",RED); \
			for(unsigned _id = 0; _id < _size_of_tab; ++_id) \
			    printf("%d ",_tab_lassert[_id]);		\
		    }							\
		    _in_case(0);					\
		    *_id_flag = 1;					\
		}else							\
		    _REQUIRE_failed(#bool);				\
		return;							\
	    }								\
	}								\
    }

/**
 * @brief Test if a pointer is not NULL (not considered an error but stop the tests)
 * @param ptr : pointer to be tested
 */
#define REQUIRE_NOT_NULL(ptr){						\
	if(*_old_flag < __LINE__){					\
	    if(!_in_case(-1))						\
		_start_test(NULL,1,0);					\
	    if(!(ptr)){							\
		if(_in_case(-1)){					\
		    *_has_to_quit = __LINE__;				\
		    printf("\n%s%s test_case :%s\n",MAGENTA,name_of_test,NORMAL); \
		    printf("\t%s%llu test(s) passed%s\n",GREEN, nb_tests(1),NORMAL); \
		    _REQUIRE_CASE_not_null_failed(#ptr);		\
		    _in_case(0);					\
		    *_id_flag = 2;					\
		}else							\
		    _REQUIRE_not_null_failed(#ptr);			\
		return;							\
	    }								\
	}								\
    }

#define EQ(VAL1,VAL2)							\
    if(VAL2 - EPSILON_LASSERT > VAL1 || VAL1 - EPSILON_LASSERT > VAL2){	\
	REQUIRE(VAL1 == VAL2);						\
    }else{								\
	REQUIRE(1);							\
    }

#define TEST_SECTION(name)						\
    void _test_##name##_lassert( char *,int *, char[512], int, int*, int*, int *); \
    void _call_test_##name##_lassert(void)				\
	__attribute__((constructor));					\
    void _call_test_##name##_lassert(void){				\
	char s[512] = {0};						\
	char log[512];							\
	double start,end;						\
	int id = -1, i = 1, old = 0;					\
	printf("%s------------------------------"			\
	       "------------------------------\n"			\
	       "BEGIN OF SECTION %s %s\n",BLUE,#name,NORMAL);		\
	_succeeded_test_case(0,1);					\
	_not_null_failed_test_case(0,1);				\
	_start_test(NULL,0,0);						\
	_failed_test_case(0,1);						\
									\
	if(using_time_asked())						\
	    start = time_used();       					\
	while(i){							\
	    i = 0;							\
	    _test_##name##_lassert(s,&id,log,0,0,&i, &old);		\
	    old = i;							\
	}								\
	if(using_time_asked())						\
	    end = time_used();						\
									\
	if(id == 1)							\
	    _failed_test_case(1,0);					\
	else if(!id)							\
	    _succeeded_test_case(1,0);					\
	else if(id == 2)						\
	    _not_null_failed_test_case(1,0);				\
	_start_test(#name,1,1);						\
	printf("\n%sEND OF SECTION %s %s\n", BLUE, #name, NORMAL);	\
	if(using_time_asked())						\
	    printf("%sExecuting section took : %fs%s\n",CYAN,INTERVAL_TIME_LASSERT(start,end),NORMAL); \
	if(_failed_test_case(0,0))					\
	    printf("%sFailed : %d test_case(s)%s\n",RED,_failed_test_case(0,0),NORMAL); \
	if(_succeeded_test_case(0,0))					\
	    printf("%sSucceeded : %d test_case(s)%s\n",GREEN,_succeeded_test_case(0,0),NORMAL); \
	if(_not_null_failed_test_case(0,0))				\
	    printf("%sStopped due to NULL pointer : %d test_case(s)%s\n",YELLOW,_not_null_failed_test_case(0,0),NORMAL); \
	if(!_succeeded_test_case(0,0) && !_failed_test_case(0,0))	\
	    printf("%sEMPTY TEST SECTION%s\n",CYAN, NORMAL);		\
	printf("%s------------------------------"			\
	       "------------------------------%s\n\n",			\
	       BLUE,NORMAL);						\
    }									\
    void _test_##name##_lassert(char * name_of_test, int * _id_flag, char _log_lassert[512], int _size_of_tab,int * _tab_lassert, int * _has_to_quit, int * _old_flag)


    
#if defined(__GNUC__) && !defined(LASSERT_MANUAL_MAIN)
/* -----------------------------
 *            GNU and auto main
 */
    int main(){
	return 0;
    }
#else
/* -----------------------------------
 *            none GNU or manual main
 */
#  define __attribute__(x)
#  define RUN_SECTION(name) _call_test_##name##_lassert()
#endif

#endif
