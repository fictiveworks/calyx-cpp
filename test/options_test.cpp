#include <catch2/catch_all.hpp>

#include <include/calyx.h>
#include <options.h>

using namespace calyx;

TEST_CASE("Strict off by default")
{
    const Options ops;
    REQUIRE_FALSE(ops.isStrict());
}

TEST_CASE("Strict on with flag set")
{
    const Options ops(true);
    REQUIRE(ops.isStrict());
}

TEST_CASE("Seeded rng is deterministic")
{
    constexpr unsigned long seed = 123;
    Options ops(seed);
    Options other(seed);

    REQUIRE(ops.randomInteger<int>() == other.randomInteger<int>());
}

TEST_CASE("Seeded rng generates a particular value")
{
    constexpr unsigned long seed = 123;
    Options ops(seed);

    REQUIRE(ops.randomInteger<int>() == 843828734);
}

