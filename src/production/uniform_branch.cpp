#include "uniform_branch.hpp"

using namespace calyx;

Expansion UniformBranch::evaluate(Options &options) const
{
    int index = options.randInt(_choices.size());
    return evaluateAt(index, options);
}

Expansion UniformBranch::evaluateAt(int index, Options &options) const
{
    Expansion tail = _choices.at(index).evaluate(options);
    return Expansion(UNIFORM_BRANCH, tail);
}

int UniformBranch::length() const
{
    return _choices.size();
}

ProductionBranch* 
UniformBranch::deepcopyBranch() const 
{
    return new UniformBranch(*this);
}