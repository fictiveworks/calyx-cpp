#include <catch2/catch_all.hpp>

#include <include/calyx.h>
#include <syntax/atom_node.hpp>

TEST_CASE("Atom tests") {
    calyx::Options ops = calyx::Options();

    calyx::AtomNode atom = calyx::AtomNode("T E R M");
    calyx::Expansion exp = atom.evaluate(ops);

    REQUIRE(calyx::Exp::ATOM == exp.getSymbol());
    REQUIRE("T E R M" == exp.getTerm());
}