#include <catch2/catch_all.hpp>
#include <memory>

#include <calyx/grammar.h>
#include <syntax/atom_node.h>
#include <errors.h>

using namespace calyx;

TEST_CASE("Atom tests") 
{
    Registry registry = Registry(Options());
    Options& ops = registry.getOptions();
    ErrorHolder errors;

    AtomNode atom = AtomNode("T E R M");
    std::optional<Expansion> exp = atom.evaluate(registry, ops, errors);

    REQUIRE(exp.has_value());
    REQUIRE_FALSE(errors.hasError());
    REQUIRE(Exp::ATOM == exp->getSymbol());
    REQUIRE("T E R M" == exp->getTerm());
}