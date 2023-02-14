#include "empty_branch.hpp"

using namespace calyx;

Expansion
EmptyBranch::evaluate(Options& options) const
{
    return evaluateAt(0, options);
}

Expansion
EmptyBranch::evaluateAt(int index, Options& options) const
{
    return Expansion(
        EMPTY_BRANCH,
        std::make_shared<Expansion>(Expansion(ATOM, options._converter.fromString("")))
    );}

int
EmptyBranch::length() const
{
    return 1;
}

ProductionBranch*
EmptyBranch::deepcopyBranch() const
{
    return new EmptyBranch(*this);
}