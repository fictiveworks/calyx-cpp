#include "empty_branch.hpp"

using namespace calyx;

Expansion EmptyBranch::evaluate(Options &options) const
{
    return Expansion(EMPTY_BRANCH, Expansion(ATOM, options._converter.fromString("")));
}

Expansion EmptyBranch::evaluateAt(int index, Options &options) const
{
    return Expansion(EMPTY_BRANCH, Expansion(ATOM, options._converter.fromString("")));
}

int EmptyBranch::length() const
{
    return 1;
}