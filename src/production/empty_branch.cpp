#include "empty_branch.hpp"

using namespace calyx;

EmptyBranch::EmptyBranch()
{
}

EmptyBranch::EmptyBranch(const EmptyBranch& old)
{
}

EmptyBranch::~EmptyBranch()
{
}

Expansion
EmptyBranch::evaluate(Options& options) const
{
    return evaluateAt(0, options);
}

Expansion
EmptyBranch::evaluateAt(int index, Options& options) const
{
    std::shared_ptr<Expansion> tail = std::make_shared<Expansion>(Expansion(ATOM, options._converter.fromString("")));

    return Expansion(EMPTY_BRANCH, tail);
}

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