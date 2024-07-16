#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

int add(int a, int b) {
    return a + b;
}

TEST_CASE("Addition") {
    REQUIRE(add(1, 2) == 3);
    REQUIRE(add(2, 3) == 5);
}
