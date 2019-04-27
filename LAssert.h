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
#  define LASSERT_UNIX
#  include <unistd.h>
#  include <sys/time.h>
#  ifdef LASSERT_CUSTOM_ALLOC
#    include <dlfcn.h>
#  endif
#  define TIME_TYPE_LASSERT double
#  define NULL_TIME_LASSERT 0
#  define TIME_LASSERT(start)						\
    long start = 0;							\
    struct timeval _timecheck_lassert;					\
    gettimeofday(&_timecheck_lassert, NULL);				\
    start = (long)_timecheck_lassert.tv_sec * 1000 + (long)_timecheck_lassert.tv_usec / 1000
#  define INTERVAL_TIME_LASSERT(start,end) (double)((end) - (start)) / 1000
#  define strcpy(a, b, c) strncpy(a, c, b)
#  define COPY(type,var) type var = var
#else
#  define LASSERT_WINDOWS
#  include <windows.h>
#  include <io.h>
#  define TIME_TYPE_LASSERT ULONGLONG
#  define NULL_TIME_LASSERT {0}
#  define TIME_LASSERT(start) TIME_TYPE_LASSERT start = GetTickCount64()
#  define INTERVAL_TIME_LASSERT(start,end) \
	((end >= start) ? (double)(end - start) : (double)((TIME_TYPE_LASSERT)-1 - start + end)) / 1000
#  define isatty(a) _isatty(a)
#  define strcpy strcpy_s
#  define dup2 _dup2
#  ifdef __cplusplus
#    define COPY(type, var) type var = ::var
#  else
#    define COPY(type, var)
#  endif
#endif

#define EPSILON_LASSERT 1e-6



/*-------------------------------------------
 * Global datas
 */
enum LASSERT_OUTPUT_OPTION{
    LASSERT_normal_output,
    LASSERT_small_output,
    LASSERT_minimized_output,
    LASSERT_xml_output
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
#elif defined(LASSERT_SMALL_OUTPUT)
    LASSERT_small_output,
#elif defined(LASSERT_XML_OUTPUT)
    LASSERT_xml_output,
#else
    LASSERT_normal_output,
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

struct{
    int failed;
    int passed;
    int testCaseOpened;
    int fdTmpFile[2];
} LASSERT_data = {0, 0, 0, {0, 0}};
/*-------------------------------------------*/






#define LASSERT_PRINT(...)                                      \
    if(LASSERT_parameters.output == LASSERT_normal_output)      \
        printf(__VA_ARGS__)

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
    LASSERT_PRINT("\n%s%s test_case :%s\n",LASSERT_MAGENTA,name_of_test,LASSERT_NORMAL);
    LASSERT_PRINT("\t%s%llu test(s) passed%s\n",LASSERT_GREEN, _nb_tests_lassert(1),LASSERT_NORMAL);
    if(statement)
	LASSERT_PRINT("\t%sFailed statement case line %d :\n\t\t%s %s\n",LASSERT_RED, line, statement,LASSERT_NORMAL);
    else
	LASSERT_PRINT("%sA test failed line %d but statement could not be read (NULL PTR)%s\n",LASSERT_RED, line, LASSERT_NORMAL);
}
void _REQUIRE_CASE_succeed(void){
    _nb_tests_lassert(0);
}
void _REQUIRE_CASE_not_null_failed(const char * ptr, int line, const char * name_of_test){
    LASSERT_PRINT("\n%s%s test_case :%s\n",LASSERT_MAGENTA,name_of_test,LASSERT_NORMAL);
    LASSERT_PRINT("\t%s%llu test(s) passed%s\n",LASSERT_GREEN, _nb_tests_lassert(1),LASSERT_NORMAL);
    if(ptr)
	LASSERT_PRINT("\t%sFailed to allocate a pointer line %d :\n\t\t%s %s\n",LASSERT_YELLOW, line, ptr,LASSERT_NORMAL);
    else
	LASSERT_PRINT("%sFailed to allocate a pointer line %d :\n\t%sCouldn't read pointer's name%s\n",LASSERT_YELLOW, line, LASSERT_RED,LASSERT_NORMAL);
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
	LASSERT_PRINT("\n%sFailed statement outside a test case line %d :\n\t%s %s\n", LASSERT_RED, line, statement,LASSERT_NORMAL);
    _failed_test_case(1,0);
}
void _REQUIRE_not_null_failed(const char * statement, int line){
    if(statement)
	LASSERT_PRINT("\n%sFailed to allocate outside a test case line %d :\n\t%s %s\n",LASSERT_YELLOW, line, statement,LASSERT_NORMAL);
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
	    LASSERT_PRINT("\n%s%s test_case :%s\n",LASSERT_MAGENTA,name_of_case,LASSERT_NORMAL);
	    LASSERT_PRINT("\t%s%llu test(s) passed%s\n",LASSERT_GREEN, _nb_tests_lassert(1),LASSERT_NORMAL);
	    LASSERT_PRINT("\t%sBad parameter in a RAND_CASE :\n\t\t%s\n\t\tSize of list should be even%s\n",LASSERT_RED,attributes,LASSERT_NORMAL);
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
	    LASSERT_PRINT("\n%s%s test_case :%s\n",LASSERT_MAGENTA,name_of_case,LASSERT_NORMAL);
	    LASSERT_PRINT("\t%s%llu test(s) passed%s\n",LASSERT_GREEN, _nb_tests_lassert(1),LASSERT_NORMAL);
	    LASSERT_PRINT("\t%sBad parameter in a RANGE_CASE :\n\t\t%s\n\t\tSize of list should be a multiple of 3%s\n",LASSERT_RED,attributes,LASSERT_NORMAL);
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
TIME_TYPE_LASSERT time_used(void){
    if(using_time_asked()){
        TIME_LASSERT(start);
        return (TIME_TYPE_LASSERT)start;
    }else{
        return NULL_TIME_LASSERT;
    }
}
int _start_test_lassert(int option,int set_start, const char * name, TIME_TYPE_LASSERT start, TIME_TYPE_LASSERT end){
    static int started = 0;

    if(option){
	if(!started){
	    started = set_start;
	}
	_nb_tests_lassert(-1);
	
        if(name){
            LASSERT_PRINT("\n%sEND OF SECTION %s %s\n", LASSERT_BLUE, name, LASSERT_NORMAL);
            if (using_time_asked()) {
                TIME_LASSERT(s);
                LASSERT_PRINT("%sExecuting section took : %fs%s\n", LASSERT_CYAN, INTERVAL_TIME_LASSERT(start, end), LASSERT_NORMAL);
            }
            if(_failed_test_case(0,0))
                LASSERT_PRINT("%sFailed : %d test_case(s)%s\n",LASSERT_RED,_failed_test_case(0,0),LASSERT_NORMAL);
            if(_succeeded_test_case(0,0))
                LASSERT_PRINT("%sSucceeded : %d test_case(s)%s\n",LASSERT_GREEN,_succeeded_test_case(0,0),LASSERT_NORMAL);
            if(_not_null_failed_test_case(0,0))
                LASSERT_PRINT("%sStopped due to NULL pointer : %d test_case(s)%s\n",LASSERT_YELLOW,_not_null_failed_test_case(0,0),LASSERT_NORMAL);
            if(!_succeeded_test_case(0,0) && !_failed_test_case(0,0))
                LASSERT_PRINT("%sEMPTY TEST SECTION%s\n",LASSERT_CYAN, LASSERT_NORMAL);
            LASSERT_PRINT("%s------------------------------------------------------------%s\n\n",LASSERT_BLUE,LASSERT_NORMAL);
        }
    }else{
	started = 0;
	LASSERT_PRINT("%s------------------------------------------------------------\n"
	       "BEGIN OF SECTION %s %s\n",LASSERT_BLUE, name, LASSERT_NORMAL);
    }

    return started;
}
void _log_message_lassert(const char * useless, ...){
    va_list vl;
    char * format;

    if(LASSERT_parameters.output == LASSERT_normal_output || LASSERT_parameters.output == LASSERT_xml_output){
        va_start(vl, useless);
        format = va_arg(vl, char *);
        vprintf(format, vl);
        va_end(vl);
    }
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
char * get_color_result_lassert(double result){
        if(result >= 100){
            return LASSERT_GREEN;
        }
        if(result >= 50){
            return LASSERT_YELLOW;
        }
        return LASSERT_RED;
}
void LASSERT_PRINT_OUTPUT(void){
    double result;
    
    if(LASSERT_parameters.output == LASSERT_minimized_output){
        printf("Percentage of test section succeeded: ");
        
        result = (double)LASSERT_data.failed / (double)(LASSERT_data.failed + LASSERT_data.passed) * 100;
        
        printf("%s%f%%%s\n", get_color_result_lassert(result), result, LASSERT_NORMAL);
    }else if(LASSERT_parameters.output == LASSERT_xml_output){
        puts("</testsuites>");
    }
}
void LASSERT_activate_output(void){
    int i;
    for(i = 0; i < 2; ++i){
        LASSERT_data.fdTmpFile[i] = mkstemp("XXXXXX");
        if(LASSERT_data.fdTmpFile[i] == -1){
            return;
        }
    }
    
    dup2(STDOUT_FILENO, LASSERT_data.fdTmpFile[0]);
    dup2(STDERR_FILENO, LASSERT_data.fdTmpFile[1]);
}
void LASSERT_deactivate_output(void){
    int i;
    for(i = 0; i < 2; ++i){
        if(LASSERT_data.fdTmpFile[i] == -1){
            return;
        }
    }

    dup2(LASSERT_data.fdTmpFile[0], STDOUT_FILENO);
    dup2(LASSERT_data.fdTmpFile[0], STDERR_FILENO);
    
    for(i = 0; i < 2; ++i){
        close(LASSERT_data.fdTmpFile[i]);
    }
}
void LASSERT_XML_PRINT(const char * s, ...){
    va_list vl;

    LASSERT_activate_output();
    
    va_start(vl, s);
    vprintf(s, vl);
    va_end(vl);

    LASSERT_deactivate_output();
};





#ifdef LASSERT_CUSTOM_ALLOC
#  ifdef LASSERT_WINDOWS
#    define LASSERT_LIB_TYPE HMODULE
#    define dlopen(a, b) LoadLibrary(a)
#    define print_error(str)                                            \
    failed = 0;                                                         \
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, \
                  NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)& err, 0, NULL); \
    if(!err){                                                           \
        failed = 1;                                                     \
        LASSERT_PRINT("%s" str "\n\t<unknown>%s\n", LASSERT_RED, LASSERT_NORMAL); \
    }else{                                                              \
        LASSERT_PRINT("%s" str "\n\t%s%s\n", LASSERT_RED, err, LASSERT_NORMAL); \
    }                                                                   \
    if(!failed){                                                        \
        LocalFree(err);                                                 \
        err = NULL;                                                     \
    }
#    define dlclose FreeLibrary
#    define DECLARE_RESOURCE(name)              \
    HGLOBAL name;                               \
    char * err = NULL;                          \
    char failed;                                \
    HRSRC name##_src
#    define HANDLE_RESOURCE(lib, name, value)                           \
    name##_src = FindResource(lib, "_lassert_malloc_lock", "int");      \
    if(!name##_src){                                                    \
        print_error("--- Failed to find LAssert lock alloc symbole");   \
    }else{                                                              \
        name = LoadResource(lib, name##_src);                           \
        if(!name){                                                      \
            print_error("--- Failed to load LAssert lock alloc symbole"); \
        }else{                                                          \
            *(int*)name = value;                                        \
        }                                                               \
    }
#  else
#    define LASSERT_LIB_TYPE void *
#    define DECLARE_RESOURCE(name) int * name
#    define print_error(str) LASSERT_PRINT("%s" str "\n\t%s%s\n", LASSERT_RED, dlerror(), LASSERT_NORMAL)
#    define HANDLE_RESOURCE(lib, name, value)                           \
    name = (int*)dlsym(lib, "_lassert_malloc_lock");                    \
    if(name){                                                           \
        *name = value;                                                  \
    }else{                                                              \
        print_error("--- Failed to load LAssert lock alloc symbole");   \
    }
#  endif
/**
 * @brief Lock/unlock allocation functions. Locked functions always return NULL
 * @param disable: flag to tell whether or not allocation functions should be locked
 */
void LAssert_alloc(int disable){
    LASSERT_LIB_TYPE lib = dlopen(LASSERT_LOCK_LIBRARY, RTLD_NOW);
    DECLARE_RESOURCE(lock);

    if(lib){
        HANDLE_RESOURCE(lib, lock, disable);
        dlclose(lib);
    }else{
        print_error("--- Failed to open LAssert lock library (" LASSERT_LOCK_LIBRARY ")");
    }
}
#endif

#define ONCE if(*_old_flag < __LINE__ && (_start_running_lassert(1) || 1)) while(_start_running_lassert(0))

#define TEST_CASE(NAME_OF_TEST)                                         \
    if(*_old_flag < __LINE__){                                          \
        if(LASSERT_parameters.output == LASSERT_xml_output){            \
            LASSERT_XML_PRINT("\t\t<testcase name="                     \
                              #NAME_OF_TEST ">");                       \
        }                                                               \
	_tab_lassert = NULL;                                            \
	if(*_id_flag == 1)                                              \
	    _failed_test_case(1,0);                                     \
	else if(!*_id_flag)                                             \
	    _succeeded_test_case(1,0);                                  \
	else if(*_id_flag == 2)                                         \
	    _not_null_failed_test_case(1,0);                            \
	_start_test_lassert(1,1, NULL, NULL_TIME_LASSERT, NULL_TIME_LASSERT); \
	strcpy(name_of_test, 512, #NAME_OF_TEST);                       \
	_start_running_lassert(1);                                      \
	_in_case_lassert(1);                                            \
	*_id_flag = 0;                                                  \
    }                                                                   \
    while(*_old_flag < __LINE__ && _start_running_lassert(0))

#define RAND_CASE(NAME_OF_TEST,var_name,nb_of_values,nb_of_time,...)    \
    int var_name[nb_of_values] = {0};					\
    int var_name##begin[nb_of_values] = {0};				\
    int var_name##end[nb_of_values] = {0};				\
    if(*_old_flag < __LINE__){						\
        if(LASSERT_parameters.output == LASSERT_xml_output){            \
            LASSERT_XML_PRINT("\t\t<testcase name="                     \
                              #NAME_OF_TEST ">");                       \
        }                                                               \
	if(*_id_flag == 1)						\
	    _failed_test_case(1,0);					\
	else if(!*_id_flag)						\
	    _succeeded_test_case(1,0);					\
	else if(*_id_flag == 2)						\
	    _not_null_failed_test_case(1,0);				\
	_start_test_lassert(1,1, NULL, NULL_TIME_LASSERT, NULL_TIME_LASSERT); \
	_size_of_tab = nb_of_values;					\
	_tab_lassert = var_name;					\
	strcpy(name_of_test, 512, #NAME_OF_TEST);                       \
	_start_running_lassert(nb_of_values);				\
	_in_case_lassert(!*_id_flag);					\
	*_id_flag = 0;							\
	_generate_tab_lassert(#NAME_OF_TEST,_id_flag,var_name##begin,var_name##end,nb_of_values,#__VA_ARGS__,__VA_ARGS__); \
    }									\
    while(*_old_flag < __LINE__ && (_next_rand_tab_lassert(var_name,var_name##begin,var_name##end,nb_of_values),_start_running_lassert(0)) && !*_id_flag)

#define RANGE_CASE(NAME_OF_TEST,var_name,nb_of_values,...)              \
    int var_name[nb_of_values] = {0};					\
    int var_name##_begin[nb_of_values] = {0};				\
    int var_name##_end[nb_of_values] = {0};				\
    int var_name##_step[nb_of_values] = {0};				\
    if(*_old_flag < __LINE__){						\
        if(LASSERT_parameters.output == LASSERT_xml_output){            \
            LASSERT_XML_PRINT("\t\t<testcase name="                     \
                              #NAME_OF_TEST ">");                       \
        }                                                               \
	_size_of_tab = nb_of_values;					\
	_tab_lassert = var_name;					\
	if(*_id_flag == 1)						\
	    _failed_test_case(1,0);					\
	else if(!*_id_flag)						\
	    _succeeded_test_case(1,0);					\
	else if(*_id_flag == 2)						\
	    _not_null_failed_test_case(1,0);				\
	_start_test_lassert(1,1, NULL, NULL_TIME_LASSERT, NULL_TIME_LASSERT); \
	strcpy(name_of_test, 512, #NAME_OF_TEST);                       \
	*_id_flag = 0;							\
	_generate_range_lassert(#NAME_OF_TEST,_id_flag,var_name,var_name##_begin,var_name##_end,var_name##_step,nb_of_values,#__VA_ARGS__,__VA_ARGS__); \
	_in_case_lassert(1);						\
    }									\
    while(*_old_flag < __LINE__ && _next_range_lassert(var_name,var_name##_begin,var_name##_end,var_name##_step,nb_of_values) && !*_id_flag)

#define LOG_MESSAGE_LASSERT(ptr, ...)					\
    if(_va_arg_not_empty_lassert(#__VA_ARGS__)){			\
	LASSERT_PRINT("\t%slog message :%s\n",LASSERT_YELLOW,LASSERT_NORMAL);  \
	_log_message_lassert(ptr, "" __VA_ARGS__);                      \
	puts(LASSERT_NORMAL);                                           \
    }

#define REQUIRE(bool,...){						\
	if(*_old_flag < __LINE__){					\
	    if(!_in_case_lassert(-1))					\
		_start_test_lassert(1,0, NULL, NULL_TIME_LASSERT, NULL_TIME_LASSERT); \
	    if(!(bool)){						\
		if(_in_case_lassert(-1)){				\
		    *_has_to_quit = __LINE__;				\
		    _REQUIRE_CASE_failed(#bool, __LINE__, name_of_test); \
		    if(_tab_lassert){					\
			LASSERT_PRINT("\t%sFailed on this sequence :\n\t\t",LASSERT_RED); \
			for(long _id = 0; _id < _size_of_tab; ++_id)    \
			    LASSERT_PRINT("%d ",_tab_lassert[_id]);     \
		    }							\
		    _in_case_lassert(0);				\
		    *_id_flag = 1;					\
		}else							\
		    _REQUIRE_failed(#bool, __LINE__);                   \
                if(LASSERT_parameters.output == LASSERT_xml_output){    \
                    LASSERT_XML_PRINT("\t\t\t<failure>\n" __VA_ARGS__); \
                    LASSERT_XML_PRINT("\t\t\t</failure>");              \
                }else{                                                  \
                    LOG_MESSAGE_LASSERT(#bool, ##__VA_ARGS__);		\
                }                                                       \
		return;							\
	    }else                                                       \
                _REQUIRE_succeed();                                     \
	}								\
    }

#define REQUIRE_NOT_NULL(ptr,...){					\
	if(*_old_flag < __LINE__){					\
	    if(!_in_case_lassert(-1))					\
		_start_test_lassert(1,0, NULL, NULL_TIME_LASSERT, NULL_TIME_LASSERT); \
	    if(!(ptr)){							\
		if(_in_case_lassert(-1)){				\
		    *_has_to_quit = __LINE__;				\
		    _REQUIRE_CASE_not_null_failed(#ptr, __LINE__, name_of_test); \
		    _in_case_lassert(0);				\
		    *_id_flag = 2;					\
		}else							\
		    _REQUIRE_not_null_failed(#ptr, __LINE__);           \
                if(LASSERT_parameters.output == LASSERT_xml_output){    \
                    LASSERT_XML_PRINT("\t\t\t<failure>\n" __VA_ARGS__); \
                    LASSERT_XML_PRINT("\t\t\t</failure>");              \
                }else{                                                  \
                    LOG_MESSAGE_LASSERT(#bool, ##__VA_ARGS__);		\
                }                                                       \
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


    
#if !defined(LASSERT_MANUAL_MAIN)
int main(){
    LASSERT_PRINT_OUTPUT();
    return LASSERT_data.failed;
}
#  ifdef LASSERT_WINDOWS
#    ifdef __cplusplus
#      define LASSERT_AUTOCALL_HANDLER(fname) ; \
          struct fname##_t_ { fname##_t_(void) { fname(); } }; static fname##_t_ fname##_;
#    else
#      pragma section(".CRT$XCU", read)
#      define LASSERT_AUTOCALL_SUB_HANDLER(fname, p) ; \
          __declspec(allocate(".CRT$XCU")) void (*fname##_)(void) = fname; \
          __pragma(comment(linker, "/include:" p #fname "_"))
#      ifdef _WIN64
#        define LASSERT_AUTOCALL_HANDLER(fname) LASSERT_AUTOCALL_SUB_HANDLER(fname, "")
#      else
#        define LASSERT_AUTOCALL_HANDLER(fname) LASSERT_AUTOCALL_SUB_HANDLER(fname, "_")
#      endif
#    endif
#  else
#    define LASSERT_AUTOCALL_HANDLER(fname) __attribute__((constructor));
#  endif
#else
#  define LASSERT_AUTOCALL_HANDLER(fname)
#  define RUN_SECTION(name) _call_test_##name##_lassert()
#endif



#define TEST_SECTION(name)						\
    void _test_##name##_lassert( char *,int *, int, int*, int*, int *); \
    static int _call_test_##name##_lassert(void)                        \
	LASSERT_AUTOCALL_HANDLER(_call_test_##name##_lassert)           \
    static int _call_test_##name##_lassert(void){                       \
	char s[512] = {0};						\
	TIME_TYPE_LASSERT start = NULL_TIME_LASSERT, end = NULL_TIME_LASSERT; \
	int id = -1, i = 1, old = 0;					\
        int failed, succeeded;                                          \
        double result;                                                  \
        LASSERT_data.testCaseOpened = 0;                                \
                                                                        \
	_succeeded_test_case(0,1);					\
	_not_null_failed_test_case(0,1);				\
	_start_test_lassert(0,0, #name, NULL_TIME_LASSERT, NULL_TIME_LASSERT); \
	_failed_test_case(0,1);						\
                                                                        \
        if(LASSERT_parameters.output == LASSERT_xml_output)             \
            LASSERT_XML_PRINT("\t<testsuite name=" #name ">");          \
	if(using_time_asked())						\
	    start = time_used();					\
	while(i){							\
	    i = 0;							\
	    _test_##name##_lassert(s,&id,0,0,&i, &old);                 \
	    old = i;							\
	}								\
	if(using_time_asked())						\
	    end = time_used();						\
        if(LASSERT_parameters.output == LASSERT_xml_output){            \
            if(LASSERT_data.testCaseOpened)                             \
                LASSERT_XML_PRINT("\t\t</testcase>");                   \
            LASSERT_XML_PRINT("\t</testsuite>");                        \
        }                                                               \
									\
	if(id == 1)							\
	    _failed_test_case(1,0);					\
	else if(!id)							\
	    _succeeded_test_case(1,0);					\
	else if(id == 2)						\
	    _not_null_failed_test_case(1,0);				\
                                                                        \
        if(LASSERT_parameters.output == LASSERT_small_output){          \
            failed = _not_null_failed_test_case(0, 0) + _failed_test_case(0, 0); \
            succeeded = _succeeded_test_case(0, 0);                     \
            if(failed + succeeded){                                     \
                result = (double)succeeded / (double)(failed + succeeded) * 100; \
                printf("%sPercentage of test cases succeeded in section " \
                       #name ": %s%f%%%s\n\n\n",                        \
                       LASSERT_BLUE, get_color_result_lassert(result),  \
                       result, LASSERT_NORMAL);                         \
            }else{                                                      \
                printf("%sSection " #name " empty%s\n\n\n",             \
                       LASSERT_BLUE, LASSERT_NORMAL);                   \
            }                                                           \
        }                                                               \
                                                                        \
	_start_test_lassert(1,1, #name, start, end);                    \
        id = 0;                                                         \
        if(_failed_test_case(0, 0))                                     \
            id = 1;                                                     \
        else if(_not_null_failed_test_case(0, 0))                       \
            id = 2;                                                     \
        if(id)                                                          \
            ++LASSERT_data.failed;                                      \
        else                                                            \
            ++LASSERT_data.passed;                                      \
                                                                        \
        return id;                                                      \
    }									\
    void _test_##name##_lassert(char * name_of_test, int * _id_flag, int _size_of_tab,int * _tab_lassert, int * _has_to_quit, int * _old_flag)




#ifndef LASSERT_MANUAL_MAIN
#  ifdef LASSERT_UNIX
void LASSERT_PARAMETERS_INIT(int argc, char** argv) __attribute__((constructor));
#  else
void LASSERT_PARAMETERS_INIT(int argc, char** argv);
static void LASSERT_PARAMETERS_SUB_INIT(void) LASSERT_AUTOCALL_HANDLER(LASSERT_PARAMETERS_SUB_INIT)
static void LASSERT_PARAMETERS_SUB_INIT(void) {
    int argc;
    LPWSTR * tmpArgv;
    char ** argv = NULL;
    int i;
    size_t dummy;
    size_t size;

    LPWSTR commandLine = GetCommandLineW();
    tmpArgv = CommandLineToArgvW(commandLine, &argc);

    argv = (char**)malloc((argc + 1) * sizeof(char *));

    if(argv){
        for(i = 0; i < argc; ++i){
            size = wcslen(tmpArgv[i]) + 1;
            argv[i] = (char*)malloc(size * sizeof(char));
            if(argv[i]){
                wcstombs_s(&dummy, argv[i], size * sizeof(char), tmpArgv[i], (size - 1) * sizeof(char));
            }
        }
        argv[argc] = NULL;
    }

    LASSERT_PARAMETERS_INIT(argc, argv);

    if(argv){
        for(i = 0; i < argc; ++i){
            if(argv[i]){
                free(argv[i]);
            }
        }
        free(argv);
    }
}
#  endif
#endif
void LASSERT_PARAMETERS_INIT(int argc, char ** argv){
    int help = 0;
    int i = 1;
    const char out[] = "-out=";
    const size_t SIZE = sizeof(out) - 1;

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
        exit(1);
    }

    if(LASSERT_parameters.output == LASSERT_xml_output){
        puts("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        puts("<testsuites name=\"LAssert\">");
        LASSERT_deactivate_output();
    }
}

#endif
