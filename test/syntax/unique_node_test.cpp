#include "syntax/unique_node.h"

#include <catch2/catch_all.hpp>

#include "calyx/grammar.h"
#include "syntax/template_node.h"

using namespace calyx;

TEST_CASE("Unique nodes cycle through each template in branch")
{
    Registry registry;
    Options& ops = registry.getOptions();
    ErrorHolder errs;

    registry.defineRule(
        ops.fromString("medal"),
        { ops.fromString("gold"), ops.fromString("silver"), ops.fromString("bronze") },
        errs
    );
    REQUIRE_FALSE(errs.hasError());

    registry.resetEvaluationContext();

    std::vector<String_t> strings;
    for (std::size_t i = 0; i < 3; i++)
    {
        UniqueNode node(ops.fromString("medal"));

        std::optional<Expansion> eval = node.evaluate(registry, ops, errs);
        REQUIRE(eval);
        REQUIRE_FALSE(errs.hasError());
        strings.push_back(eval->getTail()[0].getTail()[0].getTail()[0].getTerm());
    }

    // remove duplicates
    std::set stringsSet(strings.begin(), strings.end());

    // require that strings vector contains no duplicates
    REQUIRE(stringsSet.size() == strings.size());
}