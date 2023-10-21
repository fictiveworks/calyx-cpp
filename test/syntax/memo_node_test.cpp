#include "syntax/memo_node.h"

#include <catch2/catch_all.hpp>

#include <include/calyx.h>

using namespace calyx;

TEST_CASE("Memo nodes refer to identical symbol expansion")
{
    Registry registry;
    Options& ops = registry.getOptions();
    ErrorHolder errs;
    registry.defineRule(
        ops.fromString("one"),
        std::vector {
            ops.fromString("ONE"),
            ops.fromString("One"),
            ops.fromString("1")
        },
        errs
    );

    REQUIRE_FALSE(errs.hasError());

    MemoNode firstNode = MemoNode(ops.fromString("one"));
    MemoNode secondNode = MemoNode(ops.fromString("one"));
    MemoNode thirdNode = MemoNode(ops.fromString("one"));

    std::optional<Expansion> firstexp = firstNode.evaluate(registry, ops, errs);
    REQUIRE(firstexp);
    REQUIRE_FALSE(errs.hasError());
    std::optional<Expansion> secondexp = secondNode.evaluate(registry, ops, errs);
    REQUIRE(secondexp);
    REQUIRE_FALSE(errs.hasError());
    std::optional<Expansion> thirdexp = thirdNode.evaluate(registry, ops, errs);
    REQUIRE(thirdexp);
    REQUIRE_FALSE(errs.hasError());

    const String_t& firstTerm = firstexp->getTail()[0].getTail()[0].getTail()[0].getTerm();
    const String_t& secondTerm = secondexp->getTail()[0].getTail()[0].getTail()[0].getTerm();
    const String_t& thirdTerm = thirdexp->getTail()[0].getTail()[0].getTail()[0].getTerm();

    REQUIRE(firstTerm == thirdTerm);
    REQUIRE(firstTerm == secondTerm);
    REQUIRE(secondTerm == thirdTerm);
}