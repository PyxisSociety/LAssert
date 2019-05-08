#define LASSERT_SECTION_TIME
#include "../../LAssert.h"

TEST_SECTION(noTestCase_noFailure){
    REQUIRE(1);
    REQUIRE(1);
    REQUIRE(1);
}

TEST_SECTION(checkTests){
    CHECK(0);
    puts("Should be printed (outside case)");

    TEST_CASE(inCase){
        CHECK(0);
        puts("Should be printed (inside case)");
        REQUIRE(1);
    }

    CHECK(1, "Should not be printed");
}

TEST_SECTION(testCasesMixedUp){
    
    TEST_CASE(failing){
	unsigned i = 1;

	REQUIRE(i == 1);
	REQUIRE(i == 0);
    }

    REQUIRE(1 , "Ah que coucou Johnny");

    TEST_CASE(succeeding){
	unsigned j = 3;

	REQUIRE(j);
	REQUIRE(j);
	REQUIRE(j);
	REQUIRE(j);
    }

    TEST_CASE(failingNotNull){
	REQUIRE_NOT_NULL(NULL , "oups, null pointer");
    }
}

TEST_SECTION(empty_section){
}

int myRand(void){
    return 0;
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
    
    RAND_CASE(bad_parameters,tab2,5,5,1){
    }

    RAND_CASE(fail_on_0, tab3, 4, 4, 0, 3){
	for(unsigned i = 0; i < 4; ++i)
	    REQUIRE(tab3[i]);
    }

    LASSERT_set_rand_function(myRand);
    RAND_CASE(selfDefineRandFunction, tab4, 1, 5, 0, 1000){
        REQUIRE(!*tab4);
    }
}

TEST_SECTION(range_test){
    RANGE_CASE(should_succeed,tab,3,1,3,1){
	for(unsigned i = 0; i < 3; ++i)
	    printf("%d ",tab[i]);
	putchar('\n');
    }

    RANGE_CASE(bad_parameters,tab2,3,1){
    }
}

TEST_SECTION(copy_test){
    unsigned i = 0;

#ifndef LASSERT_WINDOWS
    TEST_CASE(should_succeed){
	COPY(unsigned,i);
	i = 2;
	REQUIRE(i);
    }
#endif

    // ONCE has to be put if there are test cases AND code not inside a test_case for the last one not to be done more than once
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

    EQ_EPS(.7, .6, .2);
}

TEST_SECTION(perfo_test){
    TEST_CASE(inside_case_ok){
        PERFORMANCE(1){
            PERFO_EXIT;
        }
    }
    
    TEST_CASE(inside_case_ko){
        puts("After this line, \"1\" should be printed");
        PERFORMANCE(1){
            while(1);
            PERFO_EXIT;
        }
        puts("1");
        REQUIRE(1);
        REQUIRE(0);
    }

    PERFORMANCE(1){
        PERFO_EXIT;
    }

    PERFORMANCE(1){
        while(1);
        PERFO_EXIT;
    }

    PERFORMANCE(1){
        PERFO_EXIT;
    }
}

#ifndef LASSERT_WINDOWS
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
