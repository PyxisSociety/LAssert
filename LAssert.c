#include "LAssert.h"

static unsigned long long nb_tests(int i){
    static unsigned long long count = 0;

    if(!i)
	++count;
    else if(i < 0)
	count = 0;

    return count;
}

/* --------- FUNCTIONS NOT TO BE CALLED DIRECTLY BY USER --------- */
void _require_failed(char * statement){
    if(statement)
	printf(RED "\tFailed statement :\n\t\t%s\n" NORMAL, statement);
    else
	puts(RED "A test failed but statement could not be read (NULL PTR)");
}

void _require_succeed(void){
    nb_tests(0);
}

void _require_not_null_failed(char * ptr){
    if(ptr)
	printf(YELLOW "\tFailed to allocate a pointer :\n\t\t%s\n" NORMAL,ptr);
    else
	puts(YELLOW "Failed to allocate a pointer :\n\t" RED "Couldn't read pointer's name" NORMAL);
}

void _test_case(void (*f)(void), char * test_func, char * test_name){
    static unsigned long long count = 1;
    
    if(f){
	printf(NORMAL "Test %llu :",count);
	if(test_name)
	    printf(" %s",test_name);
	if(test_func)
	    printf(" on %s",test_func);
	putchar('\n');
	
	nb_tests(-1);
	f();
	
	printf(GREEN "\t%llu test(s) passed\n" NORMAL, nb_tests(1));
    }else
	printf(BLUE "Test %llu : Nothing to be tested\n", count);

    putchar('\n');
    ++count;
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
	
