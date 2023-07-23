#include <catch2/catch_all.hpp>

#include <include/calyx.h>
#include <syntax/atom_node.hpp>
#include <errors.hpp>

using namespace calyx;

TEST_CASE("Atom tests") 
{
    Options ops = Options();
    ErrorHolder errors;

    AtomNode atom = AtomNode("T E R M");
    std::optional<Expansion> exp = atom.evaluate(ops, errors);

    REQUIRE(exp.has_value());
    REQUIRE_FALSE(errors.hasError());
    REQUIRE(Exp::ATOM == exp->getSymbol());
    REQUIRE("T E R M" == exp->getTerm());
}