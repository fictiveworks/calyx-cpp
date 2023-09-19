#include "empty_branch.h"

using namespace calyx;

std::optional<Expansion>
EmptyBranch::evaluate(
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{
    return evaluateAt(0, registry, options, errors);
}

std::optional<Expansion>
EmptyBranch::evaluateAt(
    std::size_t index,
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{
    auto tail = Expansion(Exp::ATOM, options.fromString(""));

    return Expansion(Exp::EMPTY_BRANCH, tail);
}

size_t
EmptyBranch::length() const
{
    return 1;
}