#include <catch2/catch_all.hpp>

#include <include/calyx.h>
#include <options.h>
#include <stdint.h>

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
    constexpr unsigned int seed = 123;
    Options ops(seed);
    Options other(seed);

    REQUIRE(ops.randomInteger<int>() == other.randomInteger<int>());
}

TEST_CASE("Seeded rng generates same value")
{
    constexpr unsigned int seed = 123;
    Options ops(seed);

    REQUIRE(ops.randomInteger<std::int32_t>() == -1303654914);
    REQUIRE(ops.randomInteger<std::int32_t>() == -1232847507);
    REQUIRE(ops.randomInteger<std::int32_t>() == 1228959102);
    REQUIRE(ops.randomInteger<std::int32_t>() == 1840268610);
}