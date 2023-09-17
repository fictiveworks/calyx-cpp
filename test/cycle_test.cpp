#include <catch2/catch_all.hpp>

#include <memory>
#include <include/calyx.h>
#include <cycle.h>

TEST_CASE("Cycle length 1 always returns 0th index") 
{
    auto ops = std::shared_ptr<calyx::Options>();
    auto errs = calyx::ErrorHolder();
    std::optional<calyx::Cycle> cycle = calyx::Cycle::create(ops, 1, errs);

    REQUIRE(cycle.has_value());
    REQUIRE_FALSE(errs.hasError());
    REQUIRE(0 == cycle->poll());
    REQUIRE(0 == cycle->poll());
    REQUIRE(0 == cycle->poll());
}


