#ifndef _LASSERT_MAIN_KLEVH_
#define _LASSERT_MAIN_KLEVH_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifndef LASSERT_LOCK_LIBRARY
#  define LASSERT_LOCK_LIBRARY "libLAssert_alloc.so"
#endif

#define LASSERT_NORMAL _get_color_lassert(0)
#define LASSERT_RED _get_color_lassert(1)
#define LASSERT_GREEN _get_color_lassert(2)
#define LASSERT_YELLOW _get_color_lassert(3)
#define LASSERT_BLUE _get_color_lassert(4)
#define LASSERT_CYAN _get_color_lassert(6)
#define LASSERT_MAGENTA _get_color_lassert(5)

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__)
#  include <unistd.h>
#  include <sys/time.h>
#  ifdef LASSERT_CUSTOM_ALLOC
#    include <dlfcn.h>
#  endif
#  define TIME_LASSERT(start)						\
    long start = 0;							\
    struct timeval _timecheck_lassert;					\
    gettimeofday(&_timecheck_lassert, NULL);				\
    start = (long)_timecheck_lassert.tv_sec * 1000 + (long)_timecheck_lassert.tv_usec / 1000
#  define INTERVAL_TIME_LASSERT(start,end) (double)((end) - (start)) / 1000
#else
#  include <windows.h>
#  define TIME_LASSERT(start)				\
    LARGE_INTEGER _frequency_lassert, start;		\
    QueryPerformanceFrequency(&_frequency_lassert);	\
    QueryPerformanceFrequency(&start)
#  define INTERVAL_TIME_LASSERT(start,end) (long double)(end.QuadPart - start.QuadPart)/_frequency_lassert.QuadPart
#endif

enum LASSERT_OUTPUT_OPTION{
    LASSERT_normal_output,
    LASSERT_small_output,
    LASSERT_minimized_output
};
enum LASSERT_COLOR_OPTION{
    LASSERT_color,
    LASSERT_no_color
};
enum LASSERT_SECTION_TIME_OPTION{
    LASSERT_no_section_time,
    LASSERT_section_time
};
    
struct{
    enum LASSERT_OUTPUT_OPTION output;
    enum LASSERT_COLOR_OPTION color;
    enum LASSERT_SECTION_TIME_OPTION timer;
    const char * filename;
} LASSERT_parameters = {
#ifdef LASSERT_MINIMIZED_OUTPUT
    LASSERT_minimized_output,
#else
#  ifdef LASSERT_SMALL_OUTPUT
    LASSERT_small_output,
#  else
    LASSERT_normal_output,
#  endif
#endif

#ifdef LASSERT_NO_COLOR
    LASSERT_no_color,
#else
    LASSERT_color,
#endif

#ifdef LASSERT_SECTION_TIME
    LASSERT_section_time
#else
    LASSERT_no_section_time
#endif
};

char * _get_color_lassert(int i){
    static char s[7][10] = {"\x1B[0m","\x1B[31m","\x1B[32m","\x1B[33m","\x1B[34m","\x1B[35m","\x1B[36m"};
    static int is_init = 0;
    unsigned j;
    
    if(!is_init){
	if(!isatty(1) || LASSERT_parameters.color == LASSERT_no_color)
	    for(j = 0; j < 7; ++j)
		s[j][0] = 0;
	is_init = 1;
    }

    return s[i];
}
unsigned long long _nb_tests_lassert(int i){
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
void _REQUIRE_CASE_failed(const char * statement, int line, const char * name_of_test){
    printf("\n%s%s test_case :%s\n",LASSERT_MAGENTA,name_of_test,LASSERT_NORMAL);
    printf("\t%s%llu test(s) passed%s\n",LASSERT_GREEN, _nb_tests_lassert(1),LASSERT_NORMAL);
    if(statement)
	printf("\t%sFailed statement case line %d :\n\t\t%s %s\n",LASSERT_RED, line, statement,LASSERT_NORMAL);
    else
	printf("%sA test failed line %d but statement could not be read (NULL PTR)%s\n",LASSERT_RED, line, LASSERT_NORMAL);
}
void _REQUIRE_CASE_succeed(void){
    _nb_tests_lassert(0);
}
void _REQUIRE_CASE_not_null_failed(const char * ptr, int line, const char * name_of_test){
    printf("\n%s%s test_case :%s\n",LASSERT_MAGENTA,name_of_test,LASSERT_NORMAL);
    printf("\t%s%llu test(s) passed%s\n",LASSERT_GREEN, _nb_tests_lassert(1),LASSERT_NORMAL);
    if(ptr)
	printf("\t%sFailed to allocate a pointer line %d :\n\t\t%s %s\n",LASSERT_YELLOW, line, ptr,LASSERT_NORMAL);
    else
	printf("%sFailed to allocate a pointer line %d :\n\t%sCouldn't read pointer's name%s\n",LASSERT_YELLOW, line, LASSERT_RED,LASSERT_NORMAL);
}
int _in_case_lassert(int option){
    static int is_in = 0;
    int i = is_in;
    if(option != -1)
	is_in = option;
    return i;
}
int _start_running_lassert(int option){
    static int old = 0;
    int res = old;
    if(option > 0){
	_in_case_lassert(1);
	old = option;
    }else if(option){
	old = 0;
    }else{
	if(old)
	    --old;
	if(!res)
	    _in_case_lassert(0);
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
void _REQUIRE_succeed(void){
    _succeeded_test_case(1,0);
}
void _REQUIRE_failed(const char * statement, int line){
    if(statement)
	printf("\n%sFailed statement outside a test case line %d :\n\t%s %s\n", LASSERT_RED, line, statement,LASSERT_NORMAL);
    _failed_test_case(1,0);
}
void _REQUIRE_not_null_failed(const char * statement, int line){
    if(statement)
	printf("\n%sFailed to allocate outside a test case line %d :\n\t%s %s\n",LASSERT_YELLOW, line, statement,LASSERT_NORMAL);
    _failed_test_case(1,0);
}
void _generate_tab_lassert(const char * name_of_case,int * _id_flag,int * begin,int * end, unsigned size, const char * attributes, ... ){
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
	    printf("\n%s%s test_case :%s\n",LASSERT_MAGENTA,name_of_case,LASSERT_NORMAL);
	    printf("\t%s%llu test(s) passed%s\n",LASSERT_GREEN, _nb_tests_lassert(1),LASSERT_NORMAL);
	    printf("\t%sBad parameter in a RAND_CASE :\n\t\t%s\n\t\tSize of list should be even%s\n",LASSERT_RED,attributes,LASSERT_NORMAL);
	    _start_running_lassert(-1);
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
void _next_rand_tab_lassert(int * tab, int * begin, int * end, unsigned size){
    for(unsigned i = 0; i < size; ++i){
	if(end[i] != begin[i])
	    tab[i] = rand()%(end[i] - begin[i]) + begin[i];
	else
	    tab[i] = end[i];
    }
}
void _generate_range_lassert(const char * name_of_case,int * _id_flag,int * tab,int * begin, int * end,int * step, unsigned size, const char * attributes, ...){
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
	    printf("\n%s%s test_case :%s\n",LASSERT_MAGENTA,name_of_case,LASSERT_NORMAL);
	    printf("\t%s%llu test(s) passed%s\n",LASSERT_GREEN, _nb_tests_lassert(1),LASSERT_NORMAL);
	    printf("\t%sBad parameter in a RANGE_CASE :\n\t\t%s\n\t\tSize of list should be a multiple of 3%s\n",LASSERT_RED,attributes,LASSERT_NORMAL);
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
int _next_range_lassert(int * tab, int * begin, int * end, int * step, size_t size){
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
	_in_case_lassert(0);
    
    return id >= 0;
}
int using_time_asked(void){
    return LASSERT_parameters.timer == LASSERT_section_time;
}
double time_used(void){
    if(using_time_asked){
        TIME_LASSERT(start);
        return (double)start;
    }else{
        return 0;
    }
}
int _start_test_lassert(int option,int set_start, const char * name, double start, double end){
    static int started = 0;

    if(option){
	if(!started){
	    started = set_start;
	}
	_nb_tests_lassert(-1);

        if(name){
            printf("\n%sEND OF SECTION %s %s\n", LASSERT_BLUE, name, LASSERT_NORMAL);
            if(using_time_asked())
                printf("%sExecuting section took : %fs%s\n",LASSERT_CYAN,INTERVAL_TIME_LASSERT(start,end),LASSERT_NORMAL);
            if(_failed_test_case(0,0))
                printf("%sFailed : %d test_case(s)%s\n",LASSERT_RED,_failed_test_case(0,0),LASSERT_NORMAL);
            if(_succeeded_test_case(0,0))
                printf("%sSucceeded : %d test_case(s)%s\n",LASSERT_GREEN,_succeeded_test_case(0,0),LASSERT_NORMAL);
            if(_not_null_failed_test_case(0,0))
                printf("%sStopped due to NULL pointer : %d test_case(s)%s\n",LASSERT_YELLOW,_not_null_failed_test_case(0,0),LASSERT_NORMAL);
            if(!_succeeded_test_case(0,0) && !_failed_test_case(0,0))
                printf("%sEMPTY TEST SECTION%s\n",LASSERT_CYAN, LASSERT_NORMAL);
            printf("%s------------------------------------------------------------%s\n\n",LASSERT_BLUE,LASSERT_NORMAL);
        }
    }else{
	started = 0;
	printf("%s------------------------------------------------------------\n"
	       "BEGIN OF SECTION %s %s\n",LASSERT_BLUE, name, LASSERT_NORMAL);
    }

    return started;
}
void _log_message_lassert(const char * useless, ...){
    va_list vl;
    char * format;

    va_start(vl, useless);
    format = va_arg(vl, char *);
    vprintf(format, vl);
    va_end(vl);
}
int _va_arg_not_empty_lassert(const char * va_arg_str){
    int i = 0;
    unsigned brackets = 0;

    while(va_arg_str[i] && (va_arg_str[i] != ',' || brackets)){
	if(va_arg_str[i] == '{' || va_arg_str[i] == '(' || va_arg_str[i] == '['){
	    ++brackets;
	}else if(brackets && (va_arg_str[i] == '}' || va_arg_str[i] == ')' || va_arg_str[i] == ']')){
	    --brackets;
	}
	++i;
    }
    ++i;

    while(va_arg_str[i] == ' ')
	++i;

    if(va_arg_str[i] == '"' && va_arg_str[i + 1] == '"'){
	i += 2;
	while(va_arg_str[i] == ' ')
	    ++i;
    }

    return va_arg_str[i] >= 10;
}




/**
 * @brief Lock/unlock allocation functions. Locked functions always return NULL
 * @param disable: flag to tell whether or not allocation functions should be locked
 */
#ifdef LASSERT_CUSTOM_ALLOC
void LAssert_alloc(int disable){
    void * lib = dlopen(LASSERT_LOCK_LIBRARY, RTLD_NOW);
    int  * lock;

    if(lib){
        lock = (int*)dlsym(lib, "_lassert_malloc_lock");

        if(lock){
            *lock = disable;
        }else{
            printf("%s--- Failed to load LAssert lock malloc symbole\n" \
                   "\t%s%s\n", LASSERT_RED, dlerror(), LASSERT_NORMAL);
        }

        dlclose(lib);
    }else{
        printf("%s--- Failed to open LAssert lock library (" LASSERT_LOCK_LIBRARY ")\n" \
               "\t%s%s\n", LASSERT_RED, dlerror(), LASSERT_NORMAL);
    }
}
#endif

#define EPSILON_LASSERT 1e-6
    
#define COPY(type,var) type var = var

#define ONCE if(*_old_flag < __LINE__ && (_start_running_lassert(1) || 1)) while(_start_running_lassert(0))

#define TEST_CASE(NAME_OF_TEST)					\
    if(*_old_flag < __LINE__){					\
	_tab_lassert = NULL;					\
	if(*_id_flag == 1)					\
	    _failed_test_case(1,0);				\
	else if(!*_id_flag)					\
	    _succeeded_test_case(1,0);				\
	else if(*_id_flag == 2)					\
	    _not_null_failed_test_case(1,0);			\
	_start_test_lassert(1,1, NULL, 0, 0);                   \
	strcpy(name_of_test,#NAME_OF_TEST);			\
	_start_running_lassert(1);				\
	_in_case_lassert(1);					\
	*_id_flag = 0;						\
    }								\
    while(*_old_flag < __LINE__ && _start_running_lassert(0))

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
	_start_test_lassert(1,1, NULL, 0, 0);                           \
	_size_of_tab = nb_of_values;					\
	_tab_lassert = var_name;					\
	strcpy(name_of_test,#NAME_OF_TEST);				\
	_start_running_lassert(nb_of_values);				\
	_in_case_lassert(!*_id_flag);					\
	*_id_flag = 0;							\
	_generate_tab_lassert(#NAME_OF_TEST,_id_flag,var_name##begin,var_name##end,nb_of_values,#ranges,ranges); \
    }									\
    while(*_old_flag < __LINE__ && (_next_rand_tab_lassert(var_name,var_name##begin,var_name##end,nb_of_values),_start_running_lassert(0)) && !*_id_flag)

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
	_start_test_lassert(1,1, NULL, 0, 0);                           \
	strcpy(name_of_test,#NAME_OF_TEST);				\
	*_id_flag = 0;							\
	_generate_range_lassert(#NAME_OF_TEST,_id_flag,var_name,var_name##_begin,var_name##_end,var_name##_step,nb_of_values,#ranges,ranges); \
	_in_case_lassert(1);						\
    }									\
    while(*_old_flag < __LINE__ && _next_range_lassert(var_name,var_name##_begin,var_name##_end,var_name##_step,nb_of_values) && !*_id_flag)

#define LOG_MESSAGE_LASSERT(ptr, ...)					\
    if(_va_arg_not_empty_lassert(#__VA_ARGS__)){			\
	printf("\t%slog message :%s\n",LASSERT_YELLOW,LASSERT_NORMAL);          \
	_log_message_lassert(ptr, "" __VA_ARGS__);                      \
	puts(LASSERT_NORMAL);                                           \
    }

#define REQUIRE(bool,...){						\
	if(*_old_flag < __LINE__){					\
	    if(!_in_case_lassert(-1))					\
		_start_test_lassert(1,0, NULL, 0, 0);                   \
	    if(!(bool)){						\
		if(_in_case_lassert(-1)){				\
		    *_has_to_quit = __LINE__;				\
		    _REQUIRE_CASE_failed(#bool, __LINE__, name_of_test); \
		    if(_tab_lassert){					\
			printf("\t%sFailed on this sequence :\n\t\t",LASSERT_RED); \
			for(long _id = 0; _id < _size_of_tab; ++_id) \
			    printf("%d ",_tab_lassert[_id]);	\
		    }							\
		    _in_case_lassert(0);				\
		    *_id_flag = 1;					\
		}else							\
		    _REQUIRE_failed(#bool, __LINE__);                   \
		LOG_MESSAGE_LASSERT(#bool, ##__VA_ARGS__);		\
		return;							\
	    }else                                                       \
                _REQUIRE_succeed();                                     \
	}								\
    }

#define REQUIRE_NOT_NULL(ptr,...){					\
	if(*_old_flag < __LINE__){					\
	    if(!_in_case_lassert(-1))					\
		_start_test_lassert(1,0, NULL, 0, 0);                   \
	    if(!(ptr)){							\
		if(_in_case_lassert(-1)){				\
		    *_has_to_quit = __LINE__;				\
		    _REQUIRE_CASE_not_null_failed(#ptr, __LINE__, name_of_test); \
		    _in_case_lassert(0);				\
		    *_id_flag = 2;					\
		}else							\
		    _REQUIRE_not_null_failed(#ptr, __LINE__);           \
		LOG_MESSAGE_LASSERT(#ptr, ##__VA_ARGS__);               \
		return;							\
	    }else                                                       \
                _REQUIRE_succeed();                                     \
	}								\
    }

#define EQ(VAL1,VAL2,...)						\
    if(VAL2 - EPSILON_LASSERT > VAL1 || VAL1 - EPSILON_LASSERT > VAL2){	\
	REQUIRE(VAL1 == VAL2,__VA_ARGS__);				\
    }else{								\
	REQUIRE(1);							\
    }

#define TEST_SECTION(name)						\
    void _test_##name##_lassert( char *,int *, int, int*, int*, int *); \
    void _call_test_##name##_lassert(void)				\
	__attribute__((constructor));					\
    void _call_test_##name##_lassert(void){				\
	char s[512] = {0};						\
	double start,end;						\
	int id = -1, i = 1, old = 0;					\
	_succeeded_test_case(0,1);					\
	_not_null_failed_test_case(0,1);				\
	_start_test_lassert(0,0, #name, 0, 0);                          \
	_failed_test_case(0,1);						\
									\
	if(using_time_asked())						\
	    start = time_used();					\
	while(i){							\
	    i = 0;							\
	    _test_##name##_lassert(s,&id,0,0,&i, &old);                 \
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
	_start_test_lassert(1,1, #name, start, end);                    \
    }									\
    void _test_##name##_lassert(char * name_of_test, int * _id_flag, int _size_of_tab,int * _tab_lassert, int * _has_to_quit, int * _old_flag)


    
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

void LASSERT_PARAMETERS_INIT(int argc, char ** argv){
    int help = 0;
    int i = 1;
    const char out[] = "-out=";
    const int SIZE = sizeof(out);

    for(; i < argc; ++i){
        if(!strcmp("-t", argv[i])){
            LASSERT_parameters.timer = LASSERT_section_time;
        }else if(!strcmp("-nt", argv[i])){
            LASSERT_parameters.timer = LASSERT_no_section_time;
        }else if(!strcmp("-c", argv[i])){
            LASSERT_parameters.color = LASSERT_color;
        }else if(!strcmp("-nc", argv[i])){
            LASSERT_parameters.color = LASSERT_no_color;
        }else if(strlen(argv[i]) >= SIZE && !strncmp(out, argv[i], SIZE)){
            if(!strcmp("small", argv[i] + SIZE)){
                LASSERT_parameters.output = LASSERT_small_output;
            }else if(!strcmp("mini", argv[i] + SIZE)){
                LASSERT_parameters.output = LASSERT_minimized_output;
            }else if(!strcmp("consol", argv[i] + SIZE)){
                LASSERT_parameters.output = LASSERT_normal_output;
            }else{
                printf("%sUNKNOWN OUTPUT OPTION [ %s%s%s ] ignored%s\n",
                       LASSERT_YELLOW, LASSERT_NORMAL, argv[i] + SIZE, LASSERT_YELLOW, LASSERT_NORMAL);
            }
        }else if(!strcmp("-h", argv[i])){
            help = 1;
        }else{
            printf("%sUNKNOWN OPTION [ %s%s%s ] ignored%s\n",
                   LASSERT_YELLOW, LASSERT_NORMAL, argv[i], LASSERT_YELLOW, LASSERT_NORMAL);
        }
    }

    if(help){
        puts("You can parametrize LASSERT to (de)activate some functionalities. All options starting with an n means to deactivate the same option without this letter.\n"
             "Here are all the available options:\n"
             "\t-[n]t: (de)activate timer functionality for sections\n"
             "\t-[n]c: (de)activate color in input (only available for UNIX systems and in consol output)\n"
             "\t-out=[option]: set the output format. [option] can be one of the following:\n"
             "\t\tconsole: standard output console with information about all test cases in all section\n"
             "\t\tsmall: smaller output giving only information about sections and not details about their test cases\n"
             "\t\tmini: (stands for minimized) no details at all, it just give the percentage of succeeded test case in every sections (as a summarized result)\n"
             "Default options are: -nt -c -out=consol"
            );
        exit(0);
    }
}

#endif
