#include <catch2/catch_all.hpp>

#include <calyx/grammar.h>
#include <expansion.h>
#include <vector>

using namespace calyx;

TEST_CASE("Wrap expression tree with result")
{
    const Result result = Result(
        Expansion(Exp::TEMPLATE, Expansion(Exp::ATOM, "A T O M"))
        );

    REQUIRE(Exp::TEMPLATE == result.getTree().getSymbol());
    REQUIRE(Exp::ATOM == result.getTree().getTail()[0].getSymbol());
    REQUIRE("A T O M" == result.getTree().getTail()[0].getTerm());
}


TEST_CASE("Flattens expression tree to string")
{
    const Expansion tripleAtomTree = Expansion(
        Exp::TEMPLATE,
        std::vector {
                Expansion(Exp::ATOM, "O N E"),
                Expansion(Exp::ATOM, " | "),
                Expansion(Exp::ATOM, "T W O")
        }
    );

    const Result result = Result(tripleAtomTree);

    const Options ops;
    std::string text = result.getText(ops);

    REQUIRE("O N E | T W O" == text);
}