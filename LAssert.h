#ifndef _LASSERT_MAIN_KLEVH_
#define _LASSERT_MAIN_KLEVH_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>

/**
 * @file LAssert.h
 * @brief header only Unitary Test Tools in C
 */

/* -------------- WARNING REMOVAL --------------- */
/* got here: https://stackoverflow.com/a/36175016 */
#define LASSERT_DIAG_STR_(s) #s
#define LASSERT_DIAG_JOINSTR_(x,y) LASSERT_DIAG_STR_(x ## y)
#ifdef _MSC_VER
#  define LASSERT_DIAG_DO_PRAGMA_(x) __pragma (x)
#  define LASSERT_DIAG_PRAGMA_(compiler,x) LASSERT_DIAG_DO_PRAGMA_(warning(x))
#else
#  define LASSERT_DIAG_DO_PRAGMA_(x) _Pragma (#x)
#  define LASSERT_DIAG_PRAGMA_(compiler,x) LASSERT_DIAG_DO_PRAGMA_(compiler diagnostic x)
#endif
#if defined(__clang__)
#  define LASSERT_DISABLE_WARNING_(gcc_unused,clang_option,msvc_unused) LASSERT_DIAG_PRAGMA_(clang,push) LASSERT_DIAG_PRAGMA_(clang,ignored LASSERT_DIAG_JOINSTR_(-W,clang_option))
#  define LASSERT_ENABLE_WARNING_(gcc_unused,clang_option,msvc_unused) LASSERT_DIAG_PRAGMA_(clang,pop)
#elif defined(_MSC_VER)
#  define LASSERT_DISABLE_WARNING_(gcc_unused,clang_unused,msvc_errorcode) LASSERT_DIAG_PRAGMA_(msvc,push) LASSERT_DIAG_DO_PRAGMA_(warning(disable:##msvc_errorcode))
#  define LASSERT_ENABLE_WARNING_(gcc_unused,clang_unused,msvc_errorcode) LASSERT_DIAG_PRAGMA_(msvc,pop)
#elif defined(__GNUC__)
#pragma GCC system_header
#  if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406
#    define LASSERT_DISABLE_WARNING_(gcc_option,clang_unused,msvc_unused) LASSERT_DIAG_PRAGMA_(GCC,push) LASSERT_DIAG_PRAGMA_(GCC,ignored LASSERT_DIAG_JOINSTR_(-W,gcc_option))
#    define LASSERT_ENABLE_WARNING_(gcc_option,clang_unused,msvc_unused) LASSERT_DIAG_PRAGMA_(GCC,pop)
#  else
#    define LASSERT_DISABLE_WARNING_(gcc_option,clang_unused,msvc_unused) LASSERT_DIAG_PRAGMA_(GCC,ignored LASSERT_DIAG_JOINSTR_(-W,gcc_option))
#    define LASSERT_ENABLE_WARNING_(gcc_option,clang_option,msvc_unused) LASSERT_DIAG_PRAGMA_(GCC,warning LASSERT_DIAG_JOINSTR_(-W,gcc_option))
#  endif
#else
#  define LASSERT_DISABLE_WARNING_(a, b, c)
#  define LASSERT_ENABLE_WARNING_(a, b, c)
#endif

LASSERT_DISABLE_WARNING_(unused-parameter, unused-parameter, 244)
LASSERT_DISABLE_WARNING_(format-security, format-security, 244)
LASSERT_DISABLE_WARNING_(unused-but-set-parameter, gnu-zero-variadic-macro-arguments, 244)
LASSERT_DISABLE_WARNING_(sign-compare, sign-compare, 244)
/* ---------------------------------------------- */




#ifndef LASSERT_LOCK_LIBRARY
#  define LASSERT_LOCK_LIBRARY "libLAssert_alloc.so"
#endif
#define LASSERT_DEFAULT_EPSILON_ 1e-6
#ifndef LASSERT_EPSILON
#  define LASSERT_EPSILON LASSERT_DEFAULT_EPSILON_
#endif
#ifndef LASSERT_SEED_
#  define LASSERT_SEED -1
#endif
#ifndef LASSERT_MAX_INFOS
#  define LASSERT_MAX_INFOS 256
#endif
#ifndef LASSERT_MAX_INFO_LENGTH
#  define LASSERT_MAX_INFO_LENGTH 4096
#endif
#if defined(LASSERT_MANUAL_MAIN) && !defined(LASSERT_MAX_SECTIONS)
#  define LASSERT_MAX_SECTIONS 1024
#endif

#define LASSERT_NORMAL_ LASSERT_get_color_(0)
#define LASSERT_RED_ LASSERT_get_color_(1)
#define LASSERT_GREEN_ LASSERT_get_color_(2)
#define LASSERT_YELLOW_ LASSERT_get_color_(3)
#define LASSERT_BLUE_ LASSERT_get_color_(4)
#define LASSERT_MAGENTA_ LASSERT_get_color_(5)
#define LASSERT_CYAN_ LASSERT_get_color_(6)

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__) || defined(__GNUC__)
#  define LASSERT_UNIX
#  define LASSERT_TMP_NAME_ "/tmp/LASSERT_XXXXXX"
#  include <sys/types.h>
#  include <sys/wait.h>
#  include <unistd.h>
#  include <sys/time.h>
#  ifdef LASSERT_CUSTOM_ALLOC
#    include <dlfcn.h>
#  endif
#  define LASSERT_TIME_TYPE_ double
#  define LASSERT_TIME_NULL_ 0
#  define LASSERT_TIME_(start)						\
    long start = 0;							\
    struct timeval _timecheck_lassert;					\
    gettimeofday(&_timecheck_lassert, NULL);				\
    start = (long)_timecheck_lassert.tv_sec * 1000 + (long)_timecheck_lassert.tv_usec / 1000
#  define LASSERT_TIME_INTERVAL_(start,end) (double)((end) - (start)) / 1000
#  define LASSERT_STRCPY_(a, b, c) strncpy(a, c, b)
#  define LASSERT_SPRINTF_(a, b, c, ...) snprintf(a, b, c, ##__VA_ARGS__)
#else
#  define LASSERT_WINDOWS
#  define LASSERT_TMP_DIR_ "C:\\Windows\\Temp\\"
#  define LASSERT_TMP_TRUE_NAME_ LASSERT_TMP_DIR_ "LASSERT_XXXXXX"
#  define LASSERT_TMP_NAME_ LASSERT_TMP_TRUE_NAME_ "                                   "
#  include <windows.h>
#  include <io.h>
#  define LASSERT_TIME_TYPE_ ULONGLONG
#  define LASSERT_TIME_NULL_ {0}
#  define LASSERT_TIME_(start) LASSERT_TIME_TYPE_ start = GetTickCount64()
#  define LASSERT_TIME_INTERVAL_(start,end)                             \
    ((end >= start) ? (double)(end - start) : (double)((LASSERT_TIME_TYPE_)-1 - start + end)) / 1000
#  define isatty(a) _isatty(a)
#  define LASSERT_STRCPY_ strcpy_s
#  define LASSERT_SPRINTF_ sprintf_s
#  define dup(a) _fdopen(_dup(_fileno(a)), "w")
#  define dup2(a, b) _dup2(_fileno(a), _fileno(b))
#  define STDOUT_FILENO stdout
#  define STDERR_FILENO stderr
#  define close fclose
#endif

#define LASSERT_LOG_MESSAGE_(ptr, ...)					\
    if(LASSERT_va_arg_not_empty_(#__VA_ARGS__)){			\
	LASSERT_PRINT_("%s", LASSERT_YELLOW_);                          \
	LASSERT_PUTS_("\tlog message :");                               \
	LASSERT_PRINT_("%s", LASSERT_NORMAL_);                          \
	LASSERT_log_message_(ptr, "" __VA_ARGS__);                      \
	LASSERT_PUTS_(LASSERT_NORMAL_);                                 \
    }
#ifdef LASSERT_MAIN
#  define LASSERT_EXTERN_
#else
#  define LASSERT_EXTERN_ extern
#endif
#define LASSERT_PRINT_LOGS_(old_line) {                                 \
        unsigned long long i;                                           \
        for(i = 0; i < LASSERT_logs_.nbMsgs; ++i){                      \
            if(!LASSERT_logs_.msgs[i].inCase || (old_line) < LASSERT_logs_.msgs[i].line){ \
                LASSERT_PRINT_("%s", LASSERT_get_color_(LASSERT_logs_.msgs[i].color)); \
                LASSERT_PUTS_(LASSERT_logs_.msgs[i].msg);               \
                LASSERT_PRINT_("%s", LASSERT_NORMAL_);                  \
            }                                                           \
        }                                                               \
        for(i = 0; i < LASSERT_logs_.nbMsgsOnce; ++i){                  \
            if(!LASSERT_logs_.msgsOnce[i].inCase || (old_line) < LASSERT_logs_.msgsOnce[i].line){ \
                LASSERT_PRINT_("%s", LASSERT_get_color_(LASSERT_logs_.msgsOnce[i].color)); \
                LASSERT_PUTS_(LASSERT_logs_.msgsOnce[i].msg);           \
                LASSERT_PRINT_("%s", LASSERT_NORMAL_);                  \
            }                                                           \
        }                                                               \
    }



/*-------------------------------------------
 * Global datas
 */
typedef enum{
    LASSERT_normal_output,
    LASSERT_small_output,
    LASSERT_minimized_output,
    LASSERT_xml_output
}LASSERT_OUTPUT_OPTION_;
typedef enum{
    LASSERT_color,
    LASSERT_no_color
}LASSERT_COLOR_OPTION_;
typedef enum{
    LASSERT_no_section_time,
    LASSERT_section_time
}LASSERT_SECTION_TIME_OPTION_;
    
typedef struct{
    LASSERT_OUTPUT_OPTION_ output;
    LASSERT_COLOR_OPTION_ color;
    LASSERT_SECTION_TIME_OPTION_ timer;
} LASSERT_PARAMETERS_;
LASSERT_EXTERN_ LASSERT_PARAMETERS_ LASSERT_parameters_
#ifdef LASSERT_MAIN
= {
# ifdef LASSERT_MINIMIZED_OUTPUT
    LASSERT_minimized_output,
# elif defined(LASSERT_SMALL_OUTPUT)
    LASSERT_small_output,
# elif defined(LASSERT_XML_OUTPUT)
    LASSERT_xml_output,
# else
    LASSERT_normal_output,
# endif

# ifdef LASSERT_NO_COLOR
    LASSERT_no_color,
# else
    LASSERT_color,
# endif

# ifdef LASSERT_SECTION_TIME
    LASSERT_section_time
# else
    LASSERT_no_section_time
# endif
}
#endif
;

typedef struct{
    char tmpFileNames[2][sizeof(LASSERT_TMP_NAME_)];
#ifdef LASSERT_WINDOWS
    FILE * fdTmpFile[2];
#else
    int fdTmpFile[2];
#endif
    double epsilon;
    long seed;
    int perfoLine;
    int inPerfoBlock;
    int failed;
    int passed;
    int testCaseOpened;
    int currentCaseFailed;
    int (*rand)(void);
    char * sectionName;
    char * tags;
    unsigned long long succeededTestsInCurrentCase;
    unsigned long long nbRunInCase;
    unsigned long long failedCases;
    unsigned long long succeededCases;
    unsigned long long notNullFailedCases;
} LASSERT_DATA_;
LASSERT_EXTERN_ LASSERT_DATA_ LASSERT_data_
#ifdef LASSERT_MAIN
= {
    {"", ""}, // tmpFileNames
    {0, 0}, // fdTmpFile
    LASSERT_EPSILON, // epsilon
    LASSERT_SEED, // seed
    0, // perfoLine
    0, // inPerfoBlock
    0, // failed
    0, // passed
    0, // testCaseOpened
    0, // currentCaseFailed
    rand, // rand
    NULL, // sectionName
    NULL, // tags
    0, // succeededTestsInCurrentCase
    0, // nbRunInCase
    0, // failedCases
    0, // succeededCases
    0 // notNullFailedCases
}
#endif
;

#ifdef LASSERT_WINDOWS
LASSERT_EXTERN_ char ** LASSERT_argv_
#  ifdef LASSERT_MAIN
= NULL
#  endif
;
LASSERT_EXTERN_ int LASSERT_argc_
#  ifdef LASSERT_MAIN
= 0
#  endif
;
#endif

typedef struct{
    void (*sectionBegin)(const char *);
    void (*sectionEnd)(const char *);
    void (*caseBegin)(const char *);
    void (*caseEnd)(const char *);
    void (*assertionFailure)(char, const char *);
}LASSERT_EVENTS_;
LASSERT_EXTERN_ LASSERT_EVENTS_ LASSERT_events_
#ifdef LASSERT_MAIN
= {NULL, NULL, NULL, NULL, NULL}
#endif
;

typedef struct{
    char msg[LASSERT_MAX_INFO_LENGTH + 20];
    char inCase;
    unsigned long long line;
    int color;
} LASSERT_LOG_INFO_;
typedef struct{
    LASSERT_LOG_INFO_ msgs[LASSERT_MAX_INFOS];
    LASSERT_LOG_INFO_ msgsOnce[LASSERT_MAX_INFOS];
    unsigned long long nbMsgs;
    unsigned long long nbMsgsOnce;
}LASSERT_LOGS_;
LASSERT_EXTERN_ LASSERT_LOGS_ LASSERT_logs_
#ifdef LASSERT_MAIN
= {
    {{{0}, 0, 0, 0}}, // msgs
    {{{0}, 0, 0, 0}}, // msgsOnce
    0, // nbMsgs
    0 // nbMsgsOnce
}
#endif
;

#ifdef LASSERT_MANUAL_MAIN
typedef struct{
    char name[256];
    int (*fun)(void);
} LASSERT_SECTION_FUNCTION_;
LASSERT_EXTERN_ LASSERT_SECTION_FUNCTION_ LASSERT_section_functions_[LASSERT_MAX_SECTIONS]
#  ifdef LASSERT_MAIN
= {{{0}, NULL}}
#  endif
;
LASSERT_EXTERN_ size_t LASSERT_section_id_
#  ifdef LASSERT_MAIN
= 0
#  endif
;
#endif
/*-------------------------------------------*/






#define LASSERT_PRINT_(...)                                     \
    if(LASSERT_parameters_.output == LASSERT_normal_output)     \
        printf(__VA_ARGS__)
#define LASSERT_PUTS_(s)                                        \
    if(LASSERT_parameters_.output == LASSERT_normal_output)     \
        puts(s)

LASSERT_EXTERN_ char * LASSERT_get_color_(int i);
LASSERT_EXTERN_ void LASSERT_init_rand_(int force);
LASSERT_EXTERN_ void LASSERT_REQUIRE_CASE_failed_(const char * statement, int line, int old_line, const char * name_of_test);
LASSERT_EXTERN_ void LASSERT_REQUIRE_CASE_not_null_failed_(const char * ptr, int line, int old_line, const char * name_of_test);
LASSERT_EXTERN_ void LASSERT_REQUIRE_failed_(const char * statement, int line, int old_line);
LASSERT_EXTERN_ void LASSERT_REQUIRE_not_null_failed_(const char * statement, int line, int old_line);
LASSERT_EXTERN_ void LASSERT_generate_tab_(const char * name_of_case,int * _id_flag,int * begin,int * end, unsigned size, const char * attributes, ... );
LASSERT_EXTERN_ void LASSERT_next_rand_tab_(int * tab, int * begin, int * end, unsigned size);
LASSERT_EXTERN_ void LASSERT_generate_range_(const char * name_of_case,int * _id_flag,int * tab,int * begin, int * end,int * step, unsigned size, const char * attributes, ...);
LASSERT_EXTERN_ int LASSERT_next_range_(int * tab, int * begin, int * end, int * step, size_t size);
LASSERT_EXTERN_ LASSERT_TIME_TYPE_ LASSERT_time_used_(void);
LASSERT_EXTERN_ void LASSERT_log_message_(const char * useless, ...);
LASSERT_EXTERN_ int LASSERT_va_arg_not_empty_(const char * va_arg_str);
LASSERT_EXTERN_ char * LASSERT_get_color_result_(double result);
LASSERT_EXTERN_ void LASSERT_deactivate_output_(void);
LASSERT_EXTERN_ void LASSERT_activate_output_(void);
LASSERT_EXTERN_ void LASSERT_XML_PRINT_(const char * s, ...);
LASSERT_EXTERN_ void LASSERT_PRINT_OUTPUT_(void);
/**
 * @brief Set the epsilon value to be used by `EQ` macro
 * @param epsilon: new epsilon value used for floating point comparison
 */
LASSERT_EXTERN_ void LASSERT_set_epsilon(double epsilon);
/**
 * @brief Set the random seed to be used (initialize standard random generator)
 * @param seed: new seed to be used with the standard random generator
 */
LASSERT_EXTERN_ void LASSERT_init_seed(unsigned seed);
/**
 * @brief Set the random function to be used
 * @param random: new function to be used to get random numbers
 */
LASSERT_EXTERN_ void LASSERT_set_rand_function(int (*random)(void));
LASSERT_EXTERN_ void LASSERT_signal_capture_(int sig);
/**
 * @brief Subscribe a function to the "SECTION begin" event
 * @param behavior: behavior to be subscribed
 */
LASSERT_EXTERN_ void LASSERT_on_section_begin(void (*behavior)(const char * name));
/**
 * @brief Subscribe a function to the "SECTION end" event
 * @param behavior: behavior to be subscribed
 */
LASSERT_EXTERN_ void LASSERT_on_section_end(void (*behavior)(const char * name));
/**
 * @brief Subscribe a function to the "CASE begin" event
 * @param behavior: behavior to be subscribed
 */
LASSERT_EXTERN_ void LASSERT_on_case_begin(void (*behavior)(const char * name));
/**
 * @brief Subscribe a function to the "CASE end" event
 * @param behavior: behavior to be subscribed
 */
LASSERT_EXTERN_ void LASSERT_on_case_end(void (*behavior)(const char * name));
/**
 * @brief Subscribe a function to the "assertion failure" event
 * @param behavior: behavior to be subscribed
 */
LASSERT_EXTERN_ void LASSERT_on_assertion_failure(void (*behavior)(char isInCase, const char * name));
LASSERT_EXTERN_ char LASSERT_is_valid_set_of_tag_(char * tags, char isCondition);
LASSERT_EXTERN_ char LASSERT_test_should_be_run_(char * testTags, char * runTags);

#ifdef LASSERT_MAIN
char * LASSERT_get_color_(int i){
#  ifdef LASSERT_UNIX
    static char s[7][10] = {"\x1B[0m","\x1B[31m","\x1B[32m","\x1B[33m","\x1B[34m","\x1B[35m","\x1B[36m"};
    static int is_init = 0;
    unsigned j;
    
    if(!is_init){
	if(!isatty(1) || LASSERT_parameters_.color == LASSERT_no_color)
	    for(j = 0; j < 7; ++j)
		s[j][0] = 0;
	is_init = 1;
    }

    return s[i];
#  else
	int colors[7] = {15, 12, 2, 14, 1, 13, 11};
	if(LASSERT_parameters_.color != LASSERT_no_color && isatty(1)){
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(h, colors[i]);
	}
	return (char*)"";
#  endif
}
void LASSERT_init_rand_(int force){
    static int not_init = 1;
    if(not_init || force){
	not_init = 0;
        if(LASSERT_data_.seed >= 0){
            srand((unsigned)LASSERT_data_.seed);
        }else{
            srand(time(NULL));
        }
    }
}
void LASSERT_REQUIRE_CASE_failed_(const char * statement, int line, int old_line, const char * name_of_test){
	LASSERT_PRINT_("%s", LASSERT_MAGENTA_);
    LASSERT_PRINT_("\n%s test_case :",name_of_test);
	LASSERT_PUTS_(LASSERT_NORMAL_);
    LASSERT_PRINT_LOGS_(old_line);
	LASSERT_PRINT_("%s", LASSERT_GREEN_);
    LASSERT_PRINT_("\t%llu test(s) passed", LASSERT_data_.succeededTestsInCurrentCase);
	LASSERT_PUTS_(LASSERT_NORMAL_);
    if(statement){
	LASSERT_PRINT_("%s", LASSERT_RED_);
	LASSERT_PRINT_("\tFailed statement case line %d :\n\t\t%s ", line, statement);
	LASSERT_PUTS_(LASSERT_NORMAL_);
    }else{
	LASSERT_PRINT_("%s", LASSERT_RED_);
	LASSERT_PRINT_("A test failed line %d but statement could not be read (NULL PTR)\n", line);
	LASSERT_PRINT_("%s", LASSERT_NORMAL_);
    }
    if(!LASSERT_data_.currentCaseFailed){
        ++LASSERT_data_.failedCases;
        LASSERT_data_.currentCaseFailed = 1;
    }
}
void LASSERT_REQUIRE_CASE_not_null_failed_(const char * ptr, int line, int old_line, const char * name_of_test){
	LASSERT_PRINT_("%s", LASSERT_MAGENTA_);
    LASSERT_PRINT_("\n%s test_case :\n",name_of_test);
	LASSERT_PRINT_("%s", LASSERT_NORMAL_);
    LASSERT_PRINT_LOGS_(old_line);
	LASSERT_PRINT_("%s", LASSERT_GREEN_);
    LASSERT_PRINT_("\t%llu test(s) passed\n", LASSERT_data_.succeededTestsInCurrentCase);
	LASSERT_PRINT_("%s", LASSERT_NORMAL_);
    if(ptr){
	LASSERT_PRINT_("%s", LASSERT_YELLOW_);
	LASSERT_PRINT_("\tFailed to allocate a pointer line %d :\n\t\t%s \n", line, ptr);
	LASSERT_PRINT_("%s", LASSERT_NORMAL_);
    }else{
	LASSERT_PRINT_("%s", LASSERT_YELLOW_);
	LASSERT_PRINT_("Failed to allocate a pointer line %d :\n\t%sCouldn't read pointer's name\n", line, LASSERT_RED_);
	LASSERT_PRINT_("%s", LASSERT_NORMAL_);
    }
    ++LASSERT_data_.notNullFailedCases;
}
void LASSERT_REQUIRE_failed_(const char * statement, int line, int old_line){
    if(statement){
        LASSERT_PUTS_("\n");
        LASSERT_PRINT_LOGS_(old_line);
		LASSERT_PRINT_("%s", LASSERT_RED_);
	LASSERT_PRINT_("Failed statement outside a test case line %d :\n\t%s \n", line, statement);
	LASSERT_PRINT_("%s", LASSERT_NORMAL_);
    }
    ++LASSERT_data_.failedCases;
}
void LASSERT_REQUIRE_not_null_failed_(const char * statement, int line, int old_line){
    if(statement){
        LASSERT_PUTS_("\n");
        LASSERT_PRINT_LOGS_(old_line);
		LASSERT_PRINT_("%s", LASSERT_YELLOW_);
	LASSERT_PRINT_("Failed to allocate outside a test case line %d :\n\t%s \n", line, statement);
	LASSERT_PRINT_("%s", LASSERT_NORMAL_);
    }
    ++LASSERT_data_.notNullFailedCases;
}
void LASSERT_generate_tab_(const char * name_of_case,int * _id_flag,int * begin,int * end, unsigned size, const char * attributes, ... ){
    va_list vl;
    unsigned number_of_parameter = 1;
    size_t id = 0,j;
    
    if(begin && end){
	LASSERT_init_rand_(0);
	while(attributes[id]){
	    if(attributes[id] == ',')
		++number_of_parameter;
	    ++id;
	}

	if(number_of_parameter&1){
		LASSERT_PRINT_("%s", LASSERT_MAGENTA_);
	    LASSERT_PRINT_("\n%s test_case :\n",name_of_case);
		LASSERT_PRINT_("%s", LASSERT_GREEN_);
	    LASSERT_PRINT_("\t%llu test(s) passed\n", LASSERT_data_.succeededTestsInCurrentCase);
		LASSERT_PRINT_("%s", LASSERT_RED_);
	    LASSERT_PRINT_("\tBad parameter in a RAND_CASE :\n\t\t%s\n\t\tSize of list should be even\n",attributes);
		LASSERT_PRINT_("%s", LASSERT_NORMAL_);
            ++LASSERT_data_.failedCases;
            LASSERT_data_.nbRunInCase = 0;
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
void LASSERT_next_rand_tab_(int * tab, int * begin, int * end, unsigned size){
    for(unsigned i = 0; i < size; ++i){
	if(end[i] != begin[i])
	    tab[i] = LASSERT_data_.rand()%(end[i] - begin[i]) + begin[i];
	else
	    tab[i] = end[i];
    }
}
void LASSERT_generate_range_(const char * name_of_case,int * _id_flag,int * tab,int * begin, int * end,int * step, unsigned size, const char * attributes, ...){
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
		LASSERT_PRINT_("%s", LASSERT_MAGENTA_);
	    LASSERT_PRINT_("\n%s test_case :\n",name_of_case);
		LASSERT_PRINT_("%s", LASSERT_GREEN_);
	    LASSERT_PRINT_("\t%llu test(s) passed\n", LASSERT_data_.succeededTestsInCurrentCase);
		LASSERT_PRINT_("%s", LASSERT_RED_);
	    LASSERT_PRINT_("\tBad parameter in a RANGE_CASE :\n\t\t%s\n\t\tSize of list should be a multiple of 3\n",attributes);
		LASSERT_PRINT_("%s", LASSERT_NORMAL_);
            ++LASSERT_data_.failedCases;
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
int LASSERT_next_range_(int * tab, int * begin, int * end, int * step, size_t size){
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
        LASSERT_data_.nbRunInCase = 0;
    
    return id >= 0;
}
LASSERT_TIME_TYPE_ LASSERT_time_used_(void){
    if(LASSERT_parameters_.timer == LASSERT_section_time){
        LASSERT_TIME_(start);
        return (LASSERT_TIME_TYPE_)start;
    }else{
		LASSERT_TIME_TYPE_ empty = LASSERT_TIME_NULL_;
        return empty;
    }
}
void LASSERT_log_message_(const char * useless, ...){
    va_list vl;
    char * format;

    if(LASSERT_parameters_.output == LASSERT_normal_output || LASSERT_parameters_.output == LASSERT_xml_output){
        va_start(vl, useless);
        format = va_arg(vl, char *);
        vprintf(format, vl);
        va_end(vl);
    }
}
int LASSERT_va_arg_not_empty_(const char * va_arg_str){
    int i = 0;

    while(va_arg_str[i] && isspace(va_arg_str[i])){
        ++i;
    }

    return va_arg_str[i];
}
char * LASSERT_get_color_result_(double result){
    if(result >= 100){
        return LASSERT_GREEN_;
    }
    if(result >= 50){
        return LASSERT_YELLOW_;
    }
    return LASSERT_RED_;
}
void LASSERT_deactivate_output_(void){
    int i;
    size_t size = sizeof(LASSERT_TMP_NAME_);
#  ifdef LASSERT_WINDOWS
    static int num = 0;
    FILE * stdCopy[2] = {NULL, NULL};
#  else
    int stdCopy[2];
#  endif
    
    for(i = 0; i < 2; ++i){
#  ifdef LASSERT_WINDOWS
        for(int j = 0; j < sizeof(LASSERT_TMP_NAME_); ++j){
            LASSERT_data_.tmpFileNames[i][j] = 0;
        }
        int e = sprintf_s(LASSERT_data_.tmpFileNames[i], sizeof(LASSERT_data_.tmpFileNames[i]) - 1, LASSERT_TMP_DIR_ "LASSERT_%d_XXXXXX", ++num);
        _mktemp_s(LASSERT_data_.tmpFileNames[i], sizeof(LASSERT_data_.tmpFileNames[i]) - 1);
        if(!*LASSERT_data_.tmpFileNames[i])
            return;
        LASSERT_data_.fdTmpFile[i] = NULL;
        fopen_s(&(LASSERT_data_.fdTmpFile[i]), LASSERT_data_.tmpFileNames[i], "w");
        if(!LASSERT_data_.fdTmpFile[i])
#  else
            LASSERT_STRCPY_(LASSERT_data_.tmpFileNames[i], size - 1, LASSERT_TMP_NAME_);
        LASSERT_data_.fdTmpFile[i] = mkstemp(LASSERT_data_.tmpFileNames[i]);
        if(LASSERT_data_.fdTmpFile[i] == -1)
#  endif
            return;
    }

    stdCopy[0] = dup(STDOUT_FILENO);
    stdCopy[1] = dup(STDERR_FILENO);
    
    dup2(LASSERT_data_.fdTmpFile[0], STDOUT_FILENO);
    dup2(LASSERT_data_.fdTmpFile[1], STDERR_FILENO);

    close(LASSERT_data_.fdTmpFile[0]);
    close(LASSERT_data_.fdTmpFile[1]);

    LASSERT_data_.fdTmpFile[0] = stdCopy[0];
    LASSERT_data_.fdTmpFile[1] = stdCopy[1];
}
void LASSERT_activate_output_(void){
    int i;
    for(i = 0; i < 2; ++i){
#  ifdef LASSERT_WINDOWS
        if(!LASSERT_data_.fdTmpFile[i])
#  else
            if(LASSERT_data_.fdTmpFile[i] == -1)
#  endif
                return;
    }
    
    dup2(LASSERT_data_.fdTmpFile[0], STDOUT_FILENO);
    dup2(LASSERT_data_.fdTmpFile[1], STDERR_FILENO);
    
    close(LASSERT_data_.fdTmpFile[0]);
    close(LASSERT_data_.fdTmpFile[1]);

    remove(LASSERT_data_.tmpFileNames[0]);
    remove(LASSERT_data_.tmpFileNames[1]);

    LASSERT_data_.fdTmpFile[0] = 0;
    LASSERT_data_.fdTmpFile[1] = 0;
}
void LASSERT_XML_PRINT_(const char * s, ...){
    va_list vl;

    if(s && *s){
        LASSERT_activate_output_();
    
        va_start(vl, s);
        vprintf(s, vl);
        va_end(vl);
        puts("");

        LASSERT_deactivate_output_();
    }
}
void LASSERT_PRINT_OUTPUT_(void){
    double result;
    
    if(LASSERT_parameters_.output == LASSERT_minimized_output){
        printf("Percentage of test section succeeded: ");
        
        result = (double)LASSERT_data_.failed / ((double)LASSERT_data_.failed + LASSERT_data_.passed) * 100;
        
		printf("%s", LASSERT_get_color_result_(result));
        printf("%f%%\n", result);
		printf("%s", LASSERT_NORMAL_);
    }else if(LASSERT_parameters_.output == LASSERT_xml_output){
        LASSERT_XML_PRINT_("</testsuites>");
        LASSERT_activate_output_();
    }

#ifdef LASSERT_WINDOWS
	if(LASSERT_argv_){
		for(int i = 0; i < LASSERT_argc_; ++i)
			free(LASSERT_argv_[i]);
		free(LASSERT_argv_);
		LASSERT_argv_ = NULL;
	}
#endif
}
void LASSERT_set_epsilon(double epsilon){
    LASSERT_data_.epsilon = epsilon;
}
void LASSERT_init_seed(unsigned seed){
    LASSERT_data_.seed = seed;
}
void LASSERT_set_rand_function(int (*random)(void)){
    if(random)
        LASSERT_data_.rand = random;
    else
        LASSERT_data_.rand = rand;
}
void LASSERT_signal_capture_(int sig){
    if(LASSERT_data_.inPerfoBlock && sig == SIGTERM){
        if(LASSERT_parameters_.output == LASSERT_xml_output){
            LASSERT_XML_PRINT_("\t\t\t<failure>");
            LASSERT_XML_PRINT_("Error in performance block starting at line %d, timeout reached",
                               LASSERT_data_.perfoLine);
            LASSERT_XML_PRINT_("\t\t\t</failure>");
        }
        exit(1);
    }
}
void LASSERT_on_section_begin(void (*behavior)(const char * name)){
    LASSERT_events_.sectionBegin = behavior;
}
void LASSERT_on_section_end(void (*behavior)(const char * name)){
    LASSERT_events_.sectionEnd = behavior;
}
void LASSERT_on_case_begin(void (*behavior)(const char * name)){
    LASSERT_events_.caseBegin = behavior;
}
void LASSERT_on_case_end(void (*behavior)(const char * name)){
    LASSERT_events_.caseEnd = behavior;
}
void LASSERT_on_assertion_failure(void (*behavior)(char isInCase, const char * name)){
    LASSERT_events_.assertionFailure = behavior;
}
char LASSERT_is_valid_set_of_tag_(char * tags, char isCondition){
    size_t i = 0;
    size_t j;
    char res = 1;
    char error = 0;

    if(!tags){
        return 0;
    }

    while(tags[i] && !error){
        // begin of tag
        if(tags[i] != '['){
            break;
        }

        // tag content
        do{
            ++i;
        }while(tags[i] && tags[i] != ']' && tags[i] != '[');

        // end of tag
        if(tags[i] != ']'){
            error = 1;
            break;
        }
        ++i;
        if(isCondition && tags[i] && tags[i] != '['){
            if(tags[i] != ','){
                break;
            }
            ++i;
            if(tags[i] != '['){
                error = 1;
                break;
            }
        }
    }

    if(tags[i] || error){
        LASSERT_PRINT_("%s\n", LASSERT_YELLOW_);
        LASSERT_PRINT_("WARNING: invalid tag\n%s\n", tags);
        for(j = 0; j < i; ++j){
            LASSERT_PRINT_("%c", ' ');
        }
        LASSERT_PUTS_("^");
        LASSERT_PRINT_("Tag is ignored");
        LASSERT_PUTS_(LASSERT_YELLOW_);
        return 0;
    }
    return 1;
}
char LASSERT_test_should_be_run_(char * testTags, char * runTags){
    size_t i;
    size_t j;
    size_t k;
    char res;
    char found;

    // parsing to know if selected
    i = 0;
    res = 0;
    while(runTags[i] && !res){
        res = 1;
        
        // see if testTags match group of tags in runTags
        while(res && runTags[i] && runTags[i] != ','){
            found = 0;
            // searching corresponding tag
            k = 0;
            while(!found && testTags[k]){
                j = i;
                while(testTags[k] != ']' && runTags[j] != ']' && runTags[j] == testTags[k]){
                    ++k;
                    ++j;
                }
                if(testTags[k] == ']' && runTags[j] == ']'){
                    found = 1;
                }else{
                    while(testTags[k] != '[' && testTags[k]){
                        ++k;
                    }
                }
            }
            
            if(!found){ // if not found, end
                res = 0;
            }else{ // if found, go to next
                i = j + 1;
            }
        }

        // If not match, go to next group of tags in runTags, if any
        if(!res){
            while(runTags[i] && runTags[i] != ','){
                ++i;
            }
            if(runTags[i]){
                ++i;
            }
        }
    }

    return res;
}
#endif





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
        LASSERT_PRINT_("%s" str "\n\t<unknown>%s\n", LASSERT_RED_, LASSERT_NORMAL_); \
    }else{                                                              \
        LASSERT_PRINT_("%s" str "\n\t%s%s\n", LASSERT_RED_, err, LASSERT_NORMAL_); \
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
#    define print_error(str) LASSERT_PRINT_("%s" str "\n\t%s%s\n", LASSERT_RED_, dlerror(), LASSERT_NORMAL_)
#    define HANDLE_RESOURCE(lib, name, value)                           \
    name = (int*)dlsym(lib, "_lassert_malloc_lock");                    \
    if(name){                                                           \
        *name = value;                                                  \
    }else{                                                              \
        print_error("--- Failed to load LAssert lock alloc symbole");   \
    }
#  endif
/**
 * @brief Lock/unlock allocation functions. Locked functions always return NULL (only on windows)
 * @param disable: flag to tell whether or not allocation functions should be locked
 */
LASSERT_EXTERN_ void LAssert_alloc(int disable);
#  ifdef LASSERT_MAIN
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
#  endif
#  undef print_error
#  undef DECLARE_RESOURCE
#  undef HANDLE_RESOURCE
#  undef LASSERT_LIB_TYPE
#  ifdef LASSERT_WINDOWS
#    undef dlclose
#    undef dlopen
#  endif
#endif





#define LASSERT_GENERIC_LOG_(COLOR, msgs, nbMsgs, message, ...)         \
    if(*_old_flag < __LINE__){                                          \
        if(LASSERT_logs_.nbMsgs < LASSERT_MAX_INFOS){                   \
            LASSERT_STRCPY_(_tmp_msg_, LASSERT_MAX_INFO_LENGTH, message); \
            LASSERT_SPRINTF_(LASSERT_logs_.msgs[LASSERT_logs_.nbMsgs].msg, \
                             LASSERT_MAX_INFO_LENGTH, _tmp_msg_,        \
                             ##__VA_ARGS__);                            \
            LASSERT_logs_.msgs[LASSERT_logs_.nbMsgs].color = COLOR;     \
            LASSERT_logs_.msgs[LASSERT_logs_.nbMsgs].inCase = LASSERT_data_.nbRunInCase; \
            LASSERT_logs_.msgs[LASSERT_logs_.nbMsgs].line = __LINE__;   \
            ++LASSERT_logs_.nbMsgs;                                     \
        }else{                                                          \
            printf("%s", LASSERT_YELLOW_);                              \
            puts(" WARNING too many logs already registered, the "      \
                 "following will be ignored:");                         \
            printf("%s\t", LASSERT_NORMAL_);                            \
            printf(message, ##__VA_ARGS__);                             \
            putchar('\n');                                              \
        }                                                               \
    }

/**
 * @brief Info to be shawn on test failure
 * @param message, ...: same parameters as fo a `printf` call
 */
#define INFO(message, ...)                                              \
    LASSERT_GENERIC_LOG_(6, msgs, nbMsgs, "INFO " message, ##__VA_ARGS__)

/**
 * @brief Warning to be shawn on test failure
 * @param message, ...: same parameters as fo a `printf` call
 */
#define WARNING(message, ...)                                           \
    LASSERT_GENERIC_LOG_(3, msgs, nbMsgs, "WARNING " message, ##__VA_ARGS__)

#ifdef ERROR // visual studio warning
#  undef ERROR
#endif
/**
 * @brief Error to be shawn on test failure
 * @param message, ...: same parameters as fo a `printf` call
 */
#define ERROR(message, ...)                                             \
    LASSERT_GENERIC_LOG_(1, msgs, nbMsgs, "ERROR " message, ##__VA_ARGS__)

/**
 * @brief Info to be shawn if the next `REQUIRE` like macro fails
 * @param message, ...: same parameters as fo a `printf` call
 */
#define INFO_ONCE(message, ...)                                         \
    LASSERT_GENERIC_LOG_(6, msgsOnce, nbMsgsOnce, "INFO " message, ##__VA_ARGS__)

/**
 * @brief Warning to be shawn if the next `REQUIRE` like macro fails
 * @param message, ...: same parameters as fo a `printf` call
 */
#define WARNING_ONCE(message, ...)                                      \
    LASSERT_GENERIC_LOG_(3, msgsOnce, nbMsgsOnce, "WARNING " message, ##__VA_ARGS__)

/**
 * @brief Error to be shawn if the next `REQUIRE` like macro fails
 * @param message, ...: same parameters as fo a `printf` call
 */
#define ERROR_ONCE(message, ...)                                        \
    LASSERT_GENERIC_LOG_(1, msgsOnce, nbMsgsOnce, "ERROR " message, ##__VA_ARGS__)

/**
 * @brief Makes sure that all modifications on a variable in the scope containing this macro call are not repercuted on the same variable in all parent scopes
 * @param type: type of the variable
 * @param var: name of the variable
 */
#define COPY(type, var) LASSERT_SUB_COPY_(type, var, __COUNTER__)
#define LASSERT_SUB_COPY_(type, var, n) LASSERT_COPY_(type, var, n)
#define LASSERT_COPY_(type, var, n)             \
    type _LASSERT_tmp_##var##_##n##_ = var;     \
    type var = _LASSERT_tmp_##var##_##n##_

/**
 * @brief Make sure that the following line (ex: `ONCE puts("ok");`, `ONCE { puts("ok"); }`) is called only once even on test failure
 */
#define ONCE if(*_old_flag < __LINE__ && (LASSERT_data_.nbRunInCase = 1)) while(LASSERT_data_.nbRunInCase--)

/**
 * @brief Macro to declare a simple test case inside a test section
 * @param NAME_OF_TEST: name of the test case
 */
#define TEST_CASE(NAME_OF_TEST)                                         \
    if(*_old_flag < __LINE__){                                          \
        if(!*_id_flag)                                                  \
            ++LASSERT_data_.succeededCases;                             \
        if(*LASSERT_name_of_test_ && LASSERT_events_.caseEnd)           \
            LASSERT_events_.caseEnd(LASSERT_name_of_test_);             \
        LASSERT_data_.succeededTestsInCurrentCase = 0;                  \
        LASSERT_data_.currentCaseFailed = 0;                            \
        if(LASSERT_parameters_.output == LASSERT_xml_output){           \
            if(LASSERT_data_.testCaseOpened){                           \
                LASSERT_XML_PRINT_("\t\t</testcase>");                  \
            }else                                                       \
                LASSERT_data_.testCaseOpened = 1;                       \
            LASSERT_XML_PRINT_("\t\t<testcase name="                    \
                               #NAME_OF_TEST ">");                      \
        }                                                               \
	_tab_lassert = NULL;                                            \
	LASSERT_STRCPY_(LASSERT_name_of_test_, 512, #NAME_OF_TEST);     \
	LASSERT_data_.nbRunInCase = 2;                                  \
	*_id_flag = 0;                                                  \
        if(LASSERT_events_.caseBegin)                                   \
            LASSERT_events_.caseBegin(#NAME_OF_TEST);                   \
    }                                                                   \
    while(*_old_flag < __LINE__ && --LASSERT_data_.nbRunInCase)

/**
 * @brief Macro to declare a values test case inside a test section (will iterate on all values of an array)
 * @param NAME_OF_TEST: name of the test case
 * @param type: type of each items of the array
 * @param variable: variable to store each items in
 * @param array: static array (in a variable) that will be iterated through
 */
#define VALUES_CASE(NAME_OF_TEST, type, variable, array)                \
    if(*_old_flag < __LINE__){                                          \
        if(!*_id_flag)                                                  \
            ++LASSERT_data_.succeededCases;                             \
        if(*LASSERT_name_of_test_ && LASSERT_events_.caseEnd)           \
            LASSERT_events_.caseEnd(LASSERT_name_of_test_);             \
        LASSERT_data_.succeededTestsInCurrentCase = 0;                  \
        LASSERT_data_.currentCaseFailed = 0;                            \
        if(LASSERT_parameters_.output == LASSERT_xml_output){           \
            if(LASSERT_data_.testCaseOpened){                           \
                LASSERT_XML_PRINT_("\t\t</testcase>");                  \
            }else                                                       \
                LASSERT_data_.testCaseOpened = 1;                       \
            LASSERT_XML_PRINT_("\t\t<testcase name="                    \
                               #NAME_OF_TEST ">");                      \
        }                                                               \
	_tab_lassert = NULL;                                            \
	LASSERT_STRCPY_(LASSERT_name_of_test_, 512, #NAME_OF_TEST);     \
	LASSERT_data_.nbRunInCase = sizeof(array) / sizeof(type) + 1;   \
	*_id_flag = 0;                                                  \
        if(LASSERT_events_.caseBegin)                                   \
            LASSERT_events_.caseBegin(#NAME_OF_TEST);                   \
    }                                                                   \
    for(type variable = array[0]; *_old_flag < __LINE__ && --LASSERT_data_.nbRunInCase; variable = array[sizeof(array) / sizeof(type) - LASSERT_data_.nbRunInCase + 1])

/**
 * @brief Macro to declare a simple random test case (integers) inside a test section
 * @param NAME_OF_TEST: name of the test case
 * @param var_name: array in which to store the random generated numbers
 * @param nb_of_values: size of var_name
 * @param nb_of_time: number of time the program should go through this test case with random values
 * @param ...: an even number of integers which can be grouped by pairs to form ranges of the values. At least one range is needed and at most nb_of_values ranges. If there are less ranges than nb_of_values, the last one is used for all other numbers. A number goes through begin to end - 1.
 */
#define RAND_CASE(NAME_OF_TEST,var_name,nb_of_values,nb_of_time,...)    \
    int var_name[nb_of_values] = {0};					\
    int var_name##begin[nb_of_values] = {0};				\
    int var_name##end[nb_of_values] = {0};				\
    if(*_old_flag < __LINE__){						\
        if(!*_id_flag)                                                  \
            ++LASSERT_data_.succeededCases;                             \
        if(*LASSERT_name_of_test_ && LASSERT_events_.caseEnd)           \
            LASSERT_events_.caseEnd(LASSERT_name_of_test_);             \
        LASSERT_data_.succeededTestsInCurrentCase = 0;                  \
        LASSERT_data_.currentCaseFailed = 0;                            \
        if(LASSERT_parameters_.output == LASSERT_xml_output){           \
            if(LASSERT_data_.testCaseOpened){                           \
                LASSERT_XML_PRINT_("\t\t</testcase>");                  \
            }else                                                       \
                LASSERT_data_.testCaseOpened = 1;                       \
            LASSERT_XML_PRINT_("\t\t<testcase name="                    \
                               #NAME_OF_TEST ">");                      \
        }                                                               \
	_size_of_tab = nb_of_values;					\
	_tab_lassert = var_name;					\
	LASSERT_STRCPY_(LASSERT_name_of_test_, 512, #NAME_OF_TEST);     \
        LASSERT_data_.nbRunInCase = nb_of_values + 1;                   \
	*_id_flag = 0;							\
        if(LASSERT_events_.caseBegin)                                   \
            LASSERT_events_.caseBegin(#NAME_OF_TEST);                   \
	LASSERT_generate_tab_(#NAME_OF_TEST,_id_flag,var_name##begin,var_name##end,nb_of_values,#__VA_ARGS__,__VA_ARGS__); \
    }									\
    while(*_old_flag < __LINE__ && !*_id_flag && (LASSERT_next_rand_tab_(var_name,var_name##begin,var_name##end,nb_of_values), --LASSERT_data_.nbRunInCase > 0))

/**
 * @brief Macro to declare a simple test case inside a test section
 * @param NAME_OF_TEST: name of the test case
 * @param var_name: array in which to store the generated numbers
 * @param nb_of_values: size of var_name
 * @param ...: a number of integers that is a multiple of 3 which can be grouped to form ranges of the values (begin, end, step). At least one range is needed and at most nb_of_values ranges. If there are less ranges than nb_of_values, the last one is used for all other numbers. A number goes through begin to end - 1.
 */
#define RANGE_CASE(NAME_OF_TEST,var_name,nb_of_values,...)              \
    int var_name[nb_of_values] = {0};					\
    int var_name##_begin[nb_of_values] = {0};				\
    int var_name##_end[nb_of_values] = {0};				\
    int var_name##_step[nb_of_values] = {0};				\
    if(*_old_flag < __LINE__){						\
        if(!*_id_flag)                                                  \
            ++LASSERT_data_.succeededCases;                             \
        if(*LASSERT_name_of_test_ && LASSERT_events_.caseEnd)           \
            LASSERT_events_.caseEnd(LASSERT_name_of_test_);             \
        LASSERT_data_.succeededTestsInCurrentCase = 0;                  \
        LASSERT_data_.currentCaseFailed = 0;                            \
        if(LASSERT_parameters_.output == LASSERT_xml_output){           \
            if(LASSERT_data_.testCaseOpened){                           \
                LASSERT_XML_PRINT_("\t\t</testcase>");                  \
            }else                                                       \
                LASSERT_data_.testCaseOpened = 1;                       \
            LASSERT_XML_PRINT_("\t\t<testcase name="                    \
                               #NAME_OF_TEST ">");                      \
        }                                                               \
	_size_of_tab = nb_of_values;					\
	_tab_lassert = var_name;					\
	LASSERT_STRCPY_(LASSERT_name_of_test_, 512, #NAME_OF_TEST);     \
	*_id_flag = 0;							\
        if(LASSERT_events_.caseBegin)                                   \
            LASSERT_events_.caseBegin(#NAME_OF_TEST);                   \
	LASSERT_generate_range_(#NAME_OF_TEST,_id_flag,var_name,var_name##_begin,var_name##_end,var_name##_step,nb_of_values,#__VA_ARGS__,__VA_ARGS__); \
	LASSERT_data_.nbRunInCase = 1;                                  \
    }									\
    while(*_old_flag < __LINE__ && LASSERT_next_range_(var_name,var_name##_begin,var_name##_end,var_name##_step,nb_of_values) && !*_id_flag)

#define LASSERT_INTERNAL_REQUIRE_(blocking, bool, ...){                 \
        int res = !!(bool);                                             \
	if(*_old_flag < __LINE__ && !*_has_to_quit){                    \
	    if(!LASSERT_data_.nbRunInCase){                             \
                if(!res && LASSERT_parameters_.output == LASSERT_xml_output){ \
                    if(LASSERT_data_.testCaseOpened){                   \
                        LASSERT_XML_PRINT_("\t\t</testcase>");          \
                    }else                                               \
                        LASSERT_data_.testCaseOpened = 1;               \
                    LASSERT_XML_PRINT_("\t\t<testcase>");               \
                }                                                       \
            }                                                           \
	    if(!res){                                                   \
                if(LASSERT_events_.assertionFailure){                   \
                    if(LASSERT_data_.nbRunInCase)                       \
                        LASSERT_events_.assertionFailure(1, LASSERT_name_of_test_); \
                    else                                                \
                        LASSERT_events_.assertionFailure(0, LASSERT_data_.sectionName); \
                }                                                       \
		if(LASSERT_data_.nbRunInCase){				\
		    if(blocking) *_has_to_quit = __LINE__;              \
		    LASSERT_REQUIRE_CASE_failed_(#bool, __LINE__, *_old_flag, LASSERT_name_of_test_); \
		    if(_tab_lassert){					\
                        LASSERT_PRINT_("%s", LASSERT_RED_);             \
			LASSERT_PRINT_("\tFailed on this sequence :\n\t\t"); \
			for(long _id = 0; _id < _size_of_tab; ++_id)    \
			    LASSERT_PRINT_("%d ",_tab_lassert[_id]);    \
		    }							\
                    LASSERT_PRINT_("%s\n", LASSERT_NORMAL_);            \
		    if(blocking) LASSERT_data_.nbRunInCase = 0;         \
		}else							\
		    LASSERT_REQUIRE_failed_(#bool, __LINE__, *_old_flag); \
                if(blocking) *_id_flag = 1;                             \
                if(LASSERT_parameters_.output == LASSERT_xml_output){   \
                    LASSERT_XML_PRINT_("\t\t\t<failure>");              \
                    LASSERT_XML_PRINT_("Error on line %d, for expression " #bool, __LINE__); \
                    LASSERT_XML_PRINT_("" __VA_ARGS__);                 \
                    LASSERT_XML_PRINT_("\t\t\t</failure>");             \
                }else{                                                  \
                    LASSERT_LOG_MESSAGE_(#bool, ##__VA_ARGS__);		\
                }                                                       \
		if(blocking) return;                                    \
	    }else if(!LASSERT_data_.nbRunInCase){                       \
                ++LASSERT_data_.succeededCases;                         \
            }else                                                       \
                ++LASSERT_data_.succeededTestsInCurrentCase;            \
            LASSERT_logs_.nbMsgsOnce = 0;                               \
	}								\
    }

/**
 * @brief Macro used inside a test section or a test case. Stop the test case / section if bool is false.
 * @param bool: condition of the REQUIRE
 * @param ... (optional): same format as printf parameters, shown on failure
 */
#define REQUIRE(bool,...) LASSERT_INTERNAL_REQUIRE_(1, bool, ##__VA_ARGS__)

/**
 * @brief Macro used inside a test section or a test case. Does not stop the test case / section if bool is false but its case will still be considered as failed.
 * @param bool: condition of the REQUIRE
 * @param ... (optional): same format as printf parameters, shown on failure
 */
#define CHECK(bool, ...) LASSERT_INTERNAL_REQUIRE_(0, bool, ##__VA_ARGS__)

/**
 * @brief Macro used inside a test section or a test case. Stop the test case / section if ptr is NULL.
 * @param ptr: pointer not to be NULL
 * @param ... (optional): same format as printf parameters, shown on failure
 */
#define REQUIRE_NOT_NULL(ptr,...){					\
	if(*_old_flag < __LINE__ && !*_has_to_quit){                    \
	    if(!LASSERT_data_.nbRunInCase){                             \
                if(!ptr && LASSERT_parameters_.output == LASSERT_xml_output){ \
                    if(LASSERT_data_.testCaseOpened){                   \
                        LASSERT_XML_PRINT_("\t\t</testcase>");          \
                    }else                                               \
                        LASSERT_data_.testCaseOpened = 1;               \
                    LASSERT_XML_PRINT_("\t\t<testcase>");               \
                }                                                       \
            }                                                           \
	    if(!(ptr)){							\
                if(LASSERT_events_.assertionFailure){                   \
                    if(LASSERT_data_.nbRunInCase)                       \
                        LASSERT_events_.assertionFailure(1, LASSERT_name_of_test_); \
                    else                                                \
                        LASSERT_events_.assertionFailure(0, LASSERT_data_.sectionName); \
                }                                                       \
		if(LASSERT_data_.nbRunInCase){				\
		    *_has_to_quit = __LINE__;				\
		    LASSERT_REQUIRE_CASE_not_null_failed_(#ptr, __LINE__, *_old_flag, LASSERT_name_of_test_); \
		    LASSERT_data_.nbRunInCase = 0;                      \
		    *_id_flag = 2;					\
		}else							\
		    LASSERT_REQUIRE_not_null_failed_(#ptr, __LINE__, *_old_flag); \
                if(LASSERT_parameters_.output == LASSERT_xml_output){   \
                    LASSERT_XML_PRINT_("\t\t\t<failure>");              \
                    LASSERT_XML_PRINT_("Error on line %d, for expression " #ptr, __LINE__); \
                    LASSERT_XML_PRINT_("" __VA_ARGS__);                 \
                    LASSERT_XML_PRINT_("\t\t\t</failure>");             \
                }else{                                                  \
                    LASSERT_LOG_MESSAGE_(#ptr, ##__VA_ARGS__);		\
                }                                                       \
		return;							\
	    }else if(!LASSERT_data_.nbRunInCase)                        \
                ++LASSERT_data_.succeededCases;                         \
            else                                                        \
                ++LASSERT_data_.succeededTestsInCurrentCase;            \
            LASSERT_logs_.nbMsgsOnce = 0;                               \
	}								\
    }

/**
 * @brief Macro used inside a test section or a test case. Stop the test case / section if VAL1 != VAL2 (with a tolarated difference of EPS).
 * @param VAL1: first value to be compared
 * @param VAL2: second value to be compared
 * @param EPS: tolarated difference between VAL1 and VAL2
 * @param ... (optional): same format as printf parameters, shown on failure
 */
#define EQ_EPS(VAL1,VAL2, EPS,...)                              \
    if((VAL2) - (EPS) > (VAL1) || (VAL1) - (EPS) > (VAL2)){     \
        REQUIRE((VAL1) == (VAL2),__VA_ARGS__);                  \
    }else{                                                      \
	REQUIRE(1);                                             \
    }

/**
 * @brief Macro used inside a test section or a test case. Stop the test case / section if VAL1 != VAL2 (with a tolarated difference).
 * @param VAL1: first value to be compared
 * @param VAL2: second value to be compared
 * @param ... (optional): same format as printf parameters, shown on failure
 */
#define EQ(VAL1,VAL2,...) EQ_EPS(VAL1, VAL2, LASSERT_data_.epsilon, ##__VA_ARGS__)

#ifndef LASSERT_WINDOWS
/**
 * @brief test of performance. This block is considered as failed if it is executed in more time than timeout seconds. It MUST be leaved with the macro PERFO_EXIT. It is *NOT* a test case so no REQUIRE, CHECK, etc should be used inside (they will produce undefined behavior).
 * @param timeout: maximum time the block has to be executed (in seconds)
 */
#  define PERFORMANCE(timeout)                                          \
    if(*_old_flag < __LINE__){						\
        fflush(stdout);                                                 \
        fflush(stderr);                                                 \
        _size_of_tab = fork();                                          \
        LASSERT_data_.perfoLine = __LINE__;                             \
    }else{                                                              \
        _size_of_tab = -2;                                              \
    }                                                                   \
    if(_size_of_tab == -1){                                             \
        printf("%s--- PERFORMANCE COULD NOT BE RUN (fork failure)\n"    \
               "\tError message: %s%s\n", LASSERT_RED_, strerror(errno), \
               LASSERT_NORMAL_);                                        \
    }else if(_size_of_tab > 0){                                         \
        sleep(timeout);                                                 \
        int i = kill(_size_of_tab, SIGTERM);                            \
        if(!i){                                                         \
            waitpid(_size_of_tab, &i, 0);                               \
            if(WIFEXITED(i)){                                           \
                if(WEXITSTATUS(i)){                                     \
                    if(LASSERT_data_.nbRunInCase){                      \
                        LASSERT_REQUIRE_CASE_failed_("timeout reached in PERFORMANCE block", \
                                                     __LINE__, *_old_flag, LASSERT_name_of_test_); \
                    }else{                                              \
                        LASSERT_REQUIRE_failed_("timeout reached in PERFORMANCE block", __LINE__, *_old_flag); \
                    }                                                   \
                }else if(!LASSERT_data_.nbRunInCase)                    \
                    ++LASSERT_data_.succeededCases;                     \
            }else{                                                      \
                printf("%s--- PERFORMANCE FAILED TO GET OUTPUT%s\n",    \
                       LASSERT_YELLOW_, LASSERT_NORMAL_);               \
            }                                                           \
        }else{                                                          \
            printf("%s--- PERFORMANCE COULD NOT BE KILLED ON TIMEOUT\n" \
                   "\tError message: %s%s\n", LASSERT_RED_, strerror(errno), \
                   LASSERT_NORMAL_);                                    \
        }                                                               \
    }else if(_size_of_tab == 0 && ((LASSERT_data_.inPerfoBlock = 1, signal(SIGTERM, LASSERT_signal_capture_)) || 1))

/**
 * @brief MACRO used to leave the PERFORMANCE block
 */
#  define PERFO_EXIT exit(0)
#endif



    
//#ifndef LASSERT_MANUAL_MAIN
#if defined(LASSERT_MAIN) && !defined(LASSERT_MANUAL_MAIN)
int main(){
    LASSERT_PRINT_OUTPUT_();
    return LASSERT_data_.failed;
}
#endif
#ifdef LASSERT_WINDOWS
#  ifdef __cplusplus
#    define LASSERT_AUTOCALL_HANDLER_(fname) ;                          \
    struct fname##_t_ { fname##_t_(void) { fname(); } }; static fname##_t_ fname##_
#  else
#    pragma section(".CRT$XCU", read)
#    define LASSERT_AUTOCALL_SUB_HANDLER_(fname, p) ;                   \
    __declspec(allocate(".CRT$XCU")) static void (*fname##_)(void) = fname
#    ifdef _WIN64
#      define LASSERT_AUTOCALL_HANDLER_(fname) LASSERT_AUTOCALL_SUB_HANDLER_(fname, "")
#    else
#      define LASSERT_AUTOCALL_HANDLER_(fname) LASSERT_AUTOCALL_SUB_HANDLER_(fname, "_")
#    endif
#  endif
#else
#  define LASSERT_AUTOCALL_HANDLER_(fname) __attribute__((constructor))
#endif
#ifndef LASSERT_MANUAL_MAIN
#  define LASSERT_AUTOCALL_(fname, sname) LASSERT_AUTOCALL_HANDLER_(fname)
#else
#  define LASSERT_AUTOCALL_(fname, sname) ;                             \
    static void _lassert_init_##fname(void)                             \
        LASSERT_AUTOCALL_HANDLER_(_lassert_init_##fname)                \
    static void _lassert_init_##fname(void){                            \
        if(LASSERT_section_id_ == LASSERT_MAX_SECTIONS){                \
            printf("%s", LASSERT_YELLOW_);                              \
            printf("WARNING too many sections in manual mode."          \
                   " Redefined LASSERT_MAX_SECTIONS to "                \
                   "enable more sections.\nSection " sname              \
                   " could not be defined");                            \
            puts(LASSERT_NORMAL_);                                      \
            return;                                                     \
        }                                                               \
        LASSERT_SECTION_FUNCTION_ tmpFun = {                            \
            sname,                                                      \
            fname                                                       \
        };                                                              \
        LASSERT_section_functions_[LASSERT_section_id_] = tmpFun;       \
        ++LASSERT_section_id_;                                          \
    }
LASSERT_EXTERN_ int LASSERT_run_section_(char * section_name)
#  ifdef LASSERT_MAIN
{
    size_t i = 0, j;
    char found = 0;

    while(i < LASSERT_section_id_ && !found){
        j = 0;
        while(section_name[j] && LASSERT_section_functions_[i].name[j] && section_name[j] == LASSERT_section_functions_[i].name[j]){
            ++j;
        }
        found = !section_name[j] && !LASSERT_section_functions_[i].name[j];
        i += !found;
    }

    if(i < LASSERT_section_id_){
        return LASSERT_section_functions_[i].fun();
    }
    return -1;
}
#  else
;
#  endif
/**
 */
#define RUN_SECTION(name) LASSERT_run_section_(#name)
#endif



/**
 * @brief main block of test (many can be defined for one test program)
 * @param ...: two parameters: name of the test section and a string containing the test section tags (optionnal)
 */
#define LASSERT_SUB_EXPAND_(x) x
#define TEST_SECTION(...) LASSERT_SUB_EXPAND_(LASSERT_CALL_SECTION_(__VA_ARGS__, LASSERT_TEST_SECTION_TAGS_, LASSERT_TEST_SECTION_NO_TAG_)(__VA_ARGS__))
#define LASSERT_CALL_SECTION_(arg0, arg1, arg2, ...) arg2
#define LASSERT_TEST_SECTION_TAGS_(name, tags) LASSERT_SUB_TEST_SECTION_(name, tags, __COUNTER__, __LINE__)
#define LASSERT_TEST_SECTION_NO_TAG_(name) LASSERT_SUB_TEST_SECTION_(name, NULL, __COUNTER__, __LINE__)
#define LASSERT_SUB_TEST_SECTION_(name, tags, number, line) LASSERT_TEST_SECTION_(name, tags, number, line)
#define LASSERT_TEST_SECTION_(name, section_tags, number, line)         \
    static void _test_##number##_##line##_lassert(char *,int *, int, int*, int*, int *, char[LASSERT_MAX_INFO_LENGTH]); \
    static int _call_test_##number##_##line##_lassert(void)             \
	LASSERT_AUTOCALL_(_call_test_##number##_##line##_lassert, #name); \
    static int _call_test_##number##_##line##_lassert(void){            \
	char s[512] = {0};						\
        char _tmp_msg_[LASSERT_MAX_INFO_LENGTH] = {0};                  \
        char * name_of_section = (char*)#name;                          \
	LASSERT_TIME_TYPE_ start = LASSERT_TIME_NULL_, end = LASSERT_TIME_NULL_; \
	int id = -1, i = 1, old = 0;					\
        int failed, succeeded;                                          \
        double result;                                                  \
        LASSERT_data_.testCaseOpened = 0;                               \
        LASSERT_data_.sectionName = name_of_section;                    \
        LASSERT_logs_.nbMsgs = 0;                                       \
                                                                        \
        if(LASSERT_data_.tags                                           \
           && (!LASSERT_is_valid_set_of_tag_(section_tags, 0)           \
               || !LASSERT_test_should_be_run_(section_tags, LASSERT_data_.tags))) \
            return 0;                                                   \
                                                                        \
        LASSERT_PRINT_("%s\n", LASSERT_BLUE_);                          \
	LASSERT_PUTS_("------------------------------------------------------------\n" \
                      "BEGIN OF SECTION " #name " ");                   \
        LASSERT_PRINT_("%s\n", LASSERT_NORMAL_);                        \
        if(LASSERT_events_.sectionBegin)                                \
            LASSERT_events_.sectionBegin(#name);                        \
                                                                        \
	LASSERT_data_.succeededCases = 0;                               \
	LASSERT_data_.notNullFailedCases = 0;				\
        LASSERT_data_.failedCases = 0;                                  \
                                                                        \
        if(LASSERT_parameters_.output == LASSERT_xml_output)            \
            LASSERT_XML_PRINT_("\t<testsuite name=" #name ">");         \
	if(LASSERT_parameters_.timer == LASSERT_section_time)           \
	    start = LASSERT_time_used_();                               \
	while(i){							\
	    i = 0;							\
	    _test_##number##_##line##_lassert(s,&id,0,0,&i, &old, _tmp_msg_); \
            if(!id){                                                    \
                ++LASSERT_data_.succeededCases;                         \
            }                                                           \
            id = -1;                                                    \
	    old = i;							\
	}								\
	if(LASSERT_parameters_.timer == LASSERT_section_time)           \
	    end = LASSERT_time_used_();                                 \
        if(LASSERT_parameters_.output == LASSERT_xml_output){           \
            if(LASSERT_data_.testCaseOpened)                            \
                LASSERT_XML_PRINT_("\t\t</testcase>");                  \
            LASSERT_XML_PRINT_("\t</testsuite>");                       \
        }                                                               \
        if(*s && LASSERT_events_.caseEnd)                               \
            LASSERT_events_.caseEnd(s);                                 \
                                                                        \
        if(LASSERT_parameters_.output == LASSERT_small_output){         \
            failed = LASSERT_data_.notNullFailedCases + LASSERT_data_.failedCases; \
            succeeded = LASSERT_data_.succeededCases;                   \
            if(failed + succeeded){                                     \
                result = (double)succeeded / ((double)failed + succeeded) * 100; \
                printf("%s", LASSERT_BLUE_);                            \
                printf("Percentage of test cases succeeded in section " \
                       #name ": %s", "");                               \
                printf("%s", LASSERT_get_color_result_(result));        \
                printf("%f%%\n\n\n", result);                           \
                printf("%s", LASSERT_NORMAL_);                          \
            }else{                                                      \
                printf("%s", LASSERT_BLUE_);                            \
                puts("Section " #name " empty\n\n");                    \
                printf("%s", LASSERT_NORMAL_);                          \
            }                                                           \
        }                                                               \
                                                                        \
        if(LASSERT_events_.sectionEnd)                                  \
            LASSERT_events_.sectionEnd(#name);                          \
        LASSERT_data_.sectionName = NULL;                               \
        LASSERT_PRINT_("%s", LASSERT_BLUE_);                            \
        LASSERT_PUTS_("\nEND OF SECTION " #name " ");                   \
        if (LASSERT_parameters_.timer == LASSERT_section_time){         \
            LASSERT_PRINT_("%s", LASSERT_CYAN_);                        \
            LASSERT_PRINT_("Executing section took : %fs\n", LASSERT_TIME_INTERVAL_(start, end)); \
        }                                                               \
        if(LASSERT_data_.failedCases){                                  \
            LASSERT_PRINT_("%s", LASSERT_RED_);                         \
            LASSERT_PRINT_("Failed : %llu test_case(s)\n", LASSERT_data_.failedCases); \
        }                                                               \
        if(LASSERT_data_.succeededCases){                               \
            LASSERT_PRINT_("%s", LASSERT_GREEN_);                       \
            LASSERT_PRINT_("Succeeded : %llu test_case(s)\n", LASSERT_data_.succeededCases); \
        }                                                               \
        if(LASSERT_data_.notNullFailedCases){                           \
            LASSERT_PRINT_("%s", LASSERT_YELLOW_);                      \
            LASSERT_PRINT_("Stopped due to NULL pointer : %llu test_case(s)\n", LASSERT_data_.notNullFailedCases); \
        }                                                               \
        if(!LASSERT_data_.succeededCases && !LASSERT_data_.failedCases){ \
            LASSERT_PRINT_("%s", LASSERT_CYAN_);                        \
            LASSERT_PUTS_("EMPTY TEST SECTION");                        \
        }                                                               \
        LASSERT_PRINT_("%s", LASSERT_BLUE_);                            \
        LASSERT_PUTS_("------------------------------------------------------------\n"); \
        LASSERT_PRINT_("%s", LASSERT_NORMAL_);                          \
                                                                        \
        id = 0;                                                         \
        if(LASSERT_data_.failedCases)                                   \
            id = 1;                                                     \
        else if(LASSERT_data_.notNullFailedCases)                       \
            id = 2;                                                     \
        if(id)                                                          \
            ++LASSERT_data_.failed;                                     \
        else                                                            \
            ++LASSERT_data_.passed;                                     \
                                                                        \
        return id;                                                      \
    }									\
    static void _test_##number##_##line##_lassert(char * LASSERT_name_of_test_, int * _id_flag, \
int _size_of_tab,int * _tab_lassert, int * _has_to_quit, int * _old_flag, char _tmp_msg_[LASSERT_MAX_INFO_LENGTH])




//#ifndef LASSERT_MANUAL_MAIN
#  ifdef LASSERT_UNIX
LASSERT_EXTERN_ void LASSERT_PARAMETERS_INIT(int argc, char** argv) __attribute__((constructor));
#  else
LASSERT_EXTERN_ void LASSERT_PARAMETERS_INIT(int argc, char** argv);
#    ifdef LASSERT_MAIN
static void LASSERT_PARAMETERS_SUB_INIT(void) LASSERT_AUTOCALL_HANDLER_(LASSERT_PARAMETERS_SUB_INIT);
    static void LASSERT_PARAMETERS_SUB_INIT(void) {
    int argc;
    LPWSTR * tmpArgv;
    int i;
    size_t dummy;
    size_t size;

    LPWSTR commandLine = GetCommandLineW();
    tmpArgv = CommandLineToArgvW(commandLine, &argc);

    LASSERT_argv_ = (char**)malloc(((unsigned long long)argc + 1) * sizeof(char *));
    LASSERT_argc_ = argc;

    if(LASSERT_argv_){
        for(i = 0; i < argc; ++i){
            size = wcslen(tmpArgv[i]) + 1;
            LASSERT_argv_[i] = (char*)malloc(size * sizeof(char));
            if(LASSERT_argv_[i]){
                wcstombs_s(&dummy, LASSERT_argv_[i], size * sizeof(char), tmpArgv[i], (size - 1) * sizeof(char));
            }
        }
        LASSERT_argv_[argc] = NULL;
    }

    LASSERT_PARAMETERS_INIT(argc, LASSERT_argv_);
}
#    endif
#  endif
/*#else
LASSERT_EXTERN_ void LASSERT_PARAMETERS_INIT(int argc, char ** argv);
#endif*/
#ifdef LASSERT_MAIN
void LASSERT_PARAMETERS_INIT(int argc, char ** argv){
    int help = 0;
    int i = 1;
    const char out[] = "-out=";
    const size_t OUT_SIZE = sizeof(out) - 1;
    const char epsilonStr[] = "-epsilon=";
    const size_t EPS_SIZE = sizeof(epsilonStr) - 1;
    const char seedStr[] = "-seed=";
    const size_t SEED_SIZE = sizeof(seedStr) - 1;
    const char tagStr[] = "-tags=";
    const size_t TAG_SIZE = sizeof(tagStr) - 1;
    long seed;
    double epsilon;

    for(; i < argc; ++i){
        if(!strcmp("-t", argv[i])){
            // -t
            LASSERT_parameters_.timer = LASSERT_section_time;
        }else if(!strcmp("-nt", argv[i])){
            // -nt
            LASSERT_parameters_.timer = LASSERT_no_section_time;
        }else if(!strcmp("-c", argv[i])){
            // -c
            LASSERT_parameters_.color = LASSERT_color;
        }else if(!strcmp("-nc", argv[i])){
            // -nc
            LASSERT_parameters_.color = LASSERT_no_color;
        }else if(strlen(argv[i]) >= TAG_SIZE && !strncmp(tagStr, argv[i], TAG_SIZE)){
            // -tags=...
            if(LASSERT_is_valid_set_of_tag_(argv[i] + TAG_SIZE, 1)){
                LASSERT_data_.tags = argv[i] + TAG_SIZE;
            }
        }else if(strlen(argv[i]) >= EPS_SIZE && !strncmp(epsilonStr, argv[i], EPS_SIZE)){
            // -epsilon=...
            epsilon = atof(argv[i] + EPS_SIZE);
            if(epsilon <= 0){
                printf("%s", LASSERT_RED_);
                puts("EPSILON VALUE MUST BE STRICTLY POSITIVE");
                printf("%s", LASSERT_YELLOW_);
                printf("%s", "\tVALUE [ ");
                printf("%s", LASSERT_NORMAL_);
                printf("%s", argv[i] + EPS_SIZE);
                printf("%s", LASSERT_YELLOW_);
                printf("%s", " ] IGNORED\n\tDEFAULT VALUE KEPT [ ");
                printf("%s", LASSERT_NORMAL_);
                printf("%f", LASSERT_EPSILON);
                printf("%s", LASSERT_YELLOW_);
                printf("%s", " ]"); 
                puts(LASSERT_NORMAL_);
            }else{
                LASSERT_data_.epsilon = epsilon;
            }
        }else if(strlen(argv[i]) >= SEED_SIZE && !strncmp(seedStr, argv[i], SEED_SIZE)){
            // -seed=...
            seed = atol(argv[i] + SEED_SIZE);
            if(seed >= 0){
                LASSERT_data_.seed = seed;
            }else{
                printf("%s", LASSERT_RED_);
                puts("SEED VALUE MUST BE AT LEAST ZERO");
                printf("%s", LASSERT_YELLOW_);
                printf("%s", "\tVALUE [ ");
                printf("%s", LASSERT_NORMAL_);
                printf("%s", argv[i] + SEED_SIZE);
                printf("%s", LASSERT_YELLOW_);
                printf("%s", " ] IGNORED\n\tDEFAULT VALUE KEPT [ ");
                printf("%s", LASSERT_NORMAL_);
                printf("%d", LASSERT_SEED);
                printf("%s", LASSERT_YELLOW_);
                printf("%s", " ]");
                puts(LASSERT_NORMAL_);
            }
        }else if(strlen(argv[i]) >= OUT_SIZE && !strncmp(out, argv[i], OUT_SIZE)){
            // -out=...
            if(!strcmp("small", argv[i] + OUT_SIZE)){
                LASSERT_parameters_.output = LASSERT_small_output;
            }else if(!strcmp("mini", argv[i] + OUT_SIZE)){
                LASSERT_parameters_.output = LASSERT_minimized_output;
            }else if(!strcmp("consol", argv[i] + OUT_SIZE)){
                LASSERT_parameters_.output = LASSERT_normal_output;
            }else if(!strcmp("xml", argv[i] + OUT_SIZE)){
                LASSERT_parameters_.output = LASSERT_xml_output;
            }else{
                printf("%s", LASSERT_YELLOW_);
                printf("%s", "UNKNOWN OUTPUT OPTION [ ");
                printf("%s", LASSERT_NORMAL_);
                printf("%s", argv[i] + OUT_SIZE);
                printf("%s", LASSERT_YELLOW_);
                printf("%s", " ] IGNORED");
                puts(LASSERT_NORMAL_);
            }
        }else if(!strcmp("-h", argv[i])){
            help = 1;
        }else{
            printf("%s", LASSERT_YELLOW_);
            printf("%s", "UNKNOWN OPTION [ ");
            printf("%s", LASSERT_NORMAL_);
            printf("%s", argv[i]);
            printf("%s", LASSERT_YELLOW_);
            printf("%s", " ] IGNORED");
            puts(LASSERT_NORMAL_);
        }
    }

#  define LASSERT_STRINGIFY_IMPL(S) #S
#  define LASSERT_STRINGIFY(S) LASSERT_STRINGIFY_IMPL(S)
    if(help){
        puts("You can parametrize LASSERT to (de)activate some functionalities. All options starting with an n means to deactivate the same option without this letter.\n"
             "Here are all the available options:\n"
             "\t-[n]t: (de)activate timer functionality for sections\n"
             "\t-[n]c: (de)activate color in input\n"
             "\t-out=[option]: set the output format. [option] can be one of the following:\n"
             "\t\tconsole: standard output console with information about all test cases in all section\n"
             "\t\tsmall: smaller output giving only information about sections and not details about their test cases\n"
             "\t\tmini: (stands for minimized) no details at all, it just give the percentage of succeeded test case in every sections (as a summarized result)\n"
             "\t\txml: JUnit like xml output\n"
             "\t-epsilon=[value]: set the accepted difference on floating point number comparison (value must be a strictly positive floating point number)\n"
             "\t-seed=[value]: set the random seed to be used to initialize\n"
             "\nDefault options are: -nt -c -out=consol -epsilon=" LASSERT_STRINGIFY(LASSERT_DEFAULT_EPSILON_)
            );
        exit(1);
    }
#  undef LASSERT_STRINGIFY
#  undef LASSERT_STRINGIFY_IMPL

    if(LASSERT_parameters_.output == LASSERT_xml_output){
        puts("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        puts("<testsuites name=\"LAssert\">");
        LASSERT_deactivate_output_();
    }
}
#endif

#ifdef LASSERT_WINDOWS
#  undef isatty
#  undef dup
#  undef dup2
#  undef close
#  undef STDOUT_FILENO
#  undef STDERR_FILENO
#endif

#endif
