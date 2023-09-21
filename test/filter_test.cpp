#include <catch2/catch_all.hpp>

#include "include/calyx.h"

using namespace calyx;

TEST_CASE("Upper case filter")
{
    Options ops;
    String_t lower = ops.fromString("string");

    String_t upper = filters::uppercase(lower, ops);

    REQUIRE(upper == ops.fromString("STRING"));
}