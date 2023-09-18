#include <catch2/catch_all.hpp>

#include <include/calyx.h>
#include <expansion.h>
#include <vector>
#include <options.h>

using namespace calyx;

TEST_CASE("Construct expansion terminal test")
{
    const Options ops;
    const String_t term = ops.fromString("T E R M");
    const auto exp = Expansion(Exp::ATOM, term);
    
    REQUIRE(exp.getTerm() == term);
}

TEST_CASE("Construct nested expansion test")
{
    const std::vector tail {
        Expansion(Exp::ATOM, "-TAHI-"),
        Expansion(Exp::ATOM, "-RUA-"),
        Expansion(Exp::ATOM, "-TORU-"),
    };
    const Expansion exp(Exp::TEMPLATE, tail);

    REQUIRE(exp.getSymbol() == Exp::TEMPLATE);
    REQUIRE(exp.getTail()[0].getSymbol() == Exp::ATOM);
    REQUIRE(exp.getTail()[0].getTerm() == "-TAHI-");
    REQUIRE(exp.getTail()[1].getSymbol() == Exp::ATOM);
    REQUIRE(exp.getTail()[1].getTerm() == "-RUA-");
    REQUIRE(exp.getTail()[2].getSymbol() == Exp::ATOM);
    REQUIRE(exp.getTail()[2].getTerm() == "-TORU-");
}

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
