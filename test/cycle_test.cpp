#include <catch2/catch_all.hpp>

#include <memory>
#include <include/calyx.h>
#include <cycle.h>

using namespace calyx;

TEST_CASE("Cycle length 1 always returns 0th index")
{
    Options ops;
    ErrorHolder errs;
    std::optional<Cycle> cycle = Cycle::create(1, ops, errs);

    REQUIRE(cycle.has_value());
    REQUIRE_FALSE(errs.hasError());
    REQUIRE(0 == cycle->poll(ops));
    REQUIRE(0 == cycle->poll(ops));
    REQUIRE(0 == cycle->poll(ops));
}