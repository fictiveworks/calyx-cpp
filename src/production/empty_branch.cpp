#include "empty_branch.hpp"

using namespace calyx;

EmptyBranch::EmptyBranch(const EmptyBranch& old)
{
}

std::optional<Expansion>
EmptyBranch::evaluate(Options& options, ErrorHolder& errors) const
{
    return evaluateAt(0, options, errors);
}

std::optional<Expansion>
EmptyBranch::evaluateAt(int index, Options& options, ErrorHolder& errors) const
{
    auto tail = Expansion(ATOM, options._converter.fromString(""));

    return Expansion(EMPTY_BRANCH, tail);
}

size_t
EmptyBranch::length() const
{
    return 1;
}

ProductionBranch*
EmptyBranch::deepcopyBranch() const
{
    return new EmptyBranch(*this);
}