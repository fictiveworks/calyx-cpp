#include <catch2/catch_all.hpp>

#include <include/calyx.h>
#include <expansion.h>
#include <vector>
#include <memory>
#include <options.h>

using namespace calyx;

TEST_CASE("Flatten expansion to atoms")
{

    std::vector<Expansion> tail;
    tail.emplace_back(Exp::ATOM, "-ONE-");
    tail.emplace_back(Exp::ATOM, "-TWO-");
    tail.emplace_back(Exp::ATOM, "-THREE-");

    auto exp = Expansion(Exp::TEMPLATE, tail);

    auto ops = Options();
    String_t atoms = exp.flatten(ops);

    REQUIRE("-ONE--TWO--THREE-" == atoms);
}
