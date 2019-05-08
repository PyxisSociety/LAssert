#include "../../LAssert.h"

TEST_SECTION(fail){
    REQUIRE(0);
}

TEST_SECTION(success){
}

TEST_SECTION(globalDataAnalysis){
    printf(__FILE__ ":\n\tfailed: %d\n\tpassed: %d\n",
           LASSERT_data_.failed, LASSERT_data_.passed);
}
