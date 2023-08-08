#include <catch2/catch_all.hpp>

#include <include/calyx.h>
#include <expansion.hpp>
#include <vector>

using namespace calyx;

TEST_CASE("Wrap expression tree with result")
{
    Result result = Result(Expansion(Exp::TEMPLATE, Expansion(Exp::ATOM, "A T O M")));

    REQUIRE(Exp::TEMPLATE == result.getTree().getSymbol());
    REQUIRE(Exp::ATOM == result.getTree().getTail()[0].getSymbol());
    REQUIRE("A T O M" == result.getTree().getTail()[0].getTerm());
}


TEST_CASE("Flattens expression tree to string")
{
    Expansion tripleAtomTree = Expansion(
        Exp::TEMPLATE,
        std::vector<Expansion> {
                Expansion(Exp::ATOM, "O N E"),
                Expansion(Exp::ATOM, " | "),
                Expansion(Exp::ATOM, "T W O")
        }
    );

    Result result = Result(tripleAtomTree);

    Options ops;
    std::string text = result.text(ops);

    REQUIRE("O N E | T W O" == text);
}