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

TEST_CASE("Seeded rng generates a particular value")
{
    constexpr unsigned int seed = 123;
    Options ops(seed);

    REQUIRE(ops.randomInteger<std::int32_t>() == 843828734);
    REQUIRE(ops.randomInteger<std::int32_t>() == 914636141);
    REQUIRE(ops.randomInteger<std::int32_t>() == -918524546);
    REQUIRE(ops.randomInteger<std::int32_t>() == -307215038);
}

