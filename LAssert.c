#include "LAssert.h"

static unsigned long long nb_tests(int i){
    static unsigned long long count = 0;

    if(!i)
	++count;
    else if(i < 0)
	count = 0;

    return count;
}

void test_case(void (*f)(void), char * test_name){
    static unsigned long long count = 1;
    
    if(f){
	printf(NORMAL "Test %llu :",count);
	if(test_name)
	    printf(" %s",test_name);
	putchar('\n');
	
	nb_tests(-1);
	f();
	
	printf(GREEN "\t%llu test(s) passed\n" NORMAL, nb_tests(1));
    }else
	printf(BLUE "Test %llu : Nothing to be tested\n", count);

    putchar('\n');
    ++count;
}

/* --------- FUNCTIONS NOT TO BE CALLED DIRECTLY BY USER --------- */
void _require_failed(char * statement){
    printf(GREEN "\t%llu test(s) passed\n",nb_tests(1));
    if(statement)
	printf(RED "\tFailed statement :\n\t\t%s\n" NORMAL, statement);
    else
	puts(RED "A test failed but statement could not be read (NULL PTR)");
    exit(-1);
}

void _require_succeed(void){
    nb_tests(0);
}

void _require_not_null_failed(char * ptr){
    printf(GREEN "\t%llu test(s) passed\n",nb_tests(1));
    if(ptr)
	printf(YELLOW "\tFailed to allocate a pointer :\n\t\t%s\n" NORMAL,ptr);
    else
	puts(YELLOW "Failed to allocate a pointer :\n\t" RED "Couldn't read pointer's name" NORMAL);
    exit(-1);
}
