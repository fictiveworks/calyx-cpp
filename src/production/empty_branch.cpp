#include "empty_branch.hpp"

using namespace calyx;

EmptyBranch::EmptyBranch(const EmptyBranch& old)
{
}

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
    int index,
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{
    auto tail = Expansion(ATOM, options.fromString(""));

    return Expansion(EMPTY_BRANCH, tail);
}

size_t
EmptyBranch::length() const
{
    return 1;
}