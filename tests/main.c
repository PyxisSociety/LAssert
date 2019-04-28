//#define LASSERT_MANUAL_MAIN

//#define LASSERT_SECTION_TIME

#include "../LAssert.h"

TEST_SECTION(noTestCase){
    REQUIRE(1);
    REQUIRE(1);
    REQUIRE(1);
}

TEST_SECTION(tralala){
    
    TEST_CASE(tralalatsointsoin){
	unsigned i = 1;

	REQUIRE(i == 1);
	REQUIRE(i == 0);
    }

    REQUIRE( 1 , "Ah que coucou Johnny" );

    TEST_CASE(tralalapastsointsoin){
	unsigned j = 3;

	REQUIRE(j);
	REQUIRE(j);
	REQUIRE(j);
	REQUIRE(j);
    }

    TEST_CASE(plop){
	REQUIRE_NOT_NULL( NULL , "oups, null pointer");
    }
}

TEST_SECTION(un_bon_gros_test){
    TEST_CASE(on_l_appel_baudhuit){
	unsigned * j = (unsigned*)malloc(sizeof(*j));

	REQUIRE_NOT_NULL(j);

	printf("j == %p\n",(void*)j);

	free(j);
    }
}

TEST_SECTION(un_test_vide){
}

TEST_SECTION(random_test){
    RAND_CASE(first_random,tab,3,3,1,10,10,100){
	puts("BEEEEGIN");
	for(unsigned i = 0;i < 3; ++i){
	    printf("%d\n",tab[i]);
	    REQUIRE(tab[i]);
	}
	putchar('\n');
    }

    RAND_CASE(should_failed,tab2,5,5,1){
    }

    RAND_CASE(fail_on_0, tab3, 4, 4, 0, 3){
	for(unsigned i = 0; i < 4; ++i)
	    REQUIRE(tab3[i]);
    }
}

TEST_SECTION(range_test){
    RANGE_CASE(should_succeed,tab,3,1,3,1){
	for(unsigned i = 0; i < 3; ++i)
	    printf("%d ",tab[i]);
	putchar('\n');
    }

    RANGE_CASE(should_failed,tab2,3,1){
    }
}

TEST_SECTION(copy_test){
    unsigned i = 0;

    TEST_CASE(should_succeed){
	COPY(unsigned,i);
	i = 2;
	REQUIRE(i);
    }

    /* ONCE has to be put if there are test cases AND code not inside a test_case for the last one not to be done more than once */
    ONCE puts("Display twice if problem not solved");

    ONCE {
	puts("A block displayed only once");
    }
    
    TEST_CASE(should_failed){
	REQUIRE(i);
    }
}

TEST_SECTION(equal_test){
    EQ( .3,.300000001);

    TEST_CASE(should_failed){
	EQ(.7,.6);
    }

    REQUIRE(0);
}

long long unsigned factorial(unsigned i){
    long long unsigned res = 1;
    for(unsigned j = 2; j < i; ++j)
	res *= j;

    return res;
}
TEST_SECTION(time_test){
    for(unsigned i = 0; i < 100; ++i)
	for(unsigned i = 0; i < 100; ++i)
	    factorial(100000);
}

#ifdef LASSERT_CUSTOM_ALLOC
TEST_SECTION(malloc_disable){
    LAssert_alloc(1);
    int * i = (int*)malloc(sizeof(*i));
    REQUIRE(!i);

    LAssert_alloc(0);
    i = (int*)malloc(sizeof(*i));
    REQUIRE_NOT_NULL(i);
    free(i);
}
#endif

#ifdef LASSERT_MANUAL_MAIN
int main(int argc, char ** argv){
    #ifdef LASSERT_SECTION_TIME
    if(argc < 2){
	RUN_SECTION(tralala);
	RUN_SECTION(un_bon_gros_test);
	RUN_SECTION(un_test_vide);
	RUN_SECTION(random_test);
	RUN_SECTION(range_test);
	RUN_SECTION(copy_test);
	RUN_SECTION(equal_test);
    }else if(argc == 2 && !strcmp(argv[1],"-t")){
	RUN_SECTION(time_test);
    }else{
	puts("Unknown set of parameters :");
	for(unsigned i = 1 + !strcmp(argv[1],"-t"); i < argc; ++i)
	    printf("\t%s\n",argv[i]);
    }
    #else
    RUN_SECTION(tralala);
    RUN_SECTION(un_bon_gros_test);
    RUN_SECTION(un_test_vide);
    RUN_SECTION(random_test);
    RUN_SECTION(range_test);
    RUN_SECTION(copy_test);
    RUN_SECTION(equal_test);
    #endif
    
    return 0;
}
#endif
