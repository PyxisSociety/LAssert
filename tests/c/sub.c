#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__) || defined(__GNUC__)
#include "../../LAssert.h"

TEST_SECTION(pouloulou){
    puts("2");
}
#endif
