#include "uniform_branch.hpp"

using namespace calyx;

UniformBranch::UniformBranch(std::vector<std::shared_ptr<Production>> choices, Registry& registry)
    : _choices(choices),
    _registry(registry)
{

}

UniformBranch::UniformBranch(const UniformBranch& old)
    : _choices(old._choices),
    _registry(old._registry)
{

}

Expansion UniformBranch::evaluate(Options& options) const
{
    int index = options.randInt(_choices.size());
    return evaluateAt(index, options);
}

Expansion UniformBranch::evaluateAt(int index, Options& options) const
{
    std::shared_ptr<Production> choice = _choices.at(index);
    Expansion tail = choice->evaluate(options);
    return Expansion(UNIFORM_BRANCH, std::make_shared<Expansion>(tail));
}

size_t UniformBranch::length() const
{
    return _choices.size();
}

ProductionBranch*
UniformBranch::deepcopyBranch() const
{
    return new UniformBranch(*this);
}