#include <catch2/catch_all.hpp>

#include <vector>
#include <string>
#include <include/calyx.h>
#include <syntax/expression_node.hpp>

using namespace calyx;

TEST_CASE("Expression test") 
{
    Registry registry;
    Options ops;

    registry.defineRule("term", {"T E R M"});

    auto expression = ExpressionNode("term", registry);

    auto exp = expression.evaluate(ops);

    REQUIRE(Exp::EXPRESSION == exp.getSymbol());

    // note that the uniform branch layer is kept here, unlike in the C# version
    // this is due to unique ptr being annoying
    REQUIRE(Exp::UNIFORM_BRANCH == exp.getTail()[0]->getSymbol());
    REQUIRE(Exp::TEMPLATE == exp.getTail()[0]->getTail()[0]->getSymbol());
    REQUIRE(Exp::ATOM == exp.getTail()[0]->getTail()[0]->getTail()[0]->getSymbol());
    REQUIRE("T E R M" == exp.getTail()[0]->getTail()[0]->getTail()[0]->getTerm());
}