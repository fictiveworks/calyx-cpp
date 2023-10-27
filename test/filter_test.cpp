#include <catch2/catch_all.hpp>

#include "include/calyx.h"

using namespace calyx;

TEST_CASE("Upper case filter")
{
    const Options ops;
    const String_t base = ops.fromString("String");

    const String_t upper = BuiltinFilters::uppercase(base, ops);

    REQUIRE(upper == ops.fromString("STRING"));
}

TEST_CASE("Lower case filter")
{
    const Options ops;
    const String_t base = ops.fromString("String");

    const String_t lower = BuiltinFilters::lowercase(base, ops);

    REQUIRE(lower == ops.fromString("string"));
}

TEST_CASE("Emphasis filter")
{
    const Options ops;
    const String_t base = ops.fromString("String");

    const String_t filtered = BuiltinFilters::emphasis(base, ops);

    REQUIRE(filtered == ops.fromString("*String*"));
}

TEST_CASE("Length filter")
{
    const Options ops;
    const String_t base = ops.fromString("String");

    const String_t filtered = BuiltinFilters::length(base, ops);

    REQUIRE(filtered == ops.fromString("6"));
}