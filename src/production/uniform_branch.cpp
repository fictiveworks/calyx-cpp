#include "uniform_branch.hpp"
#include "../syntax/template_node.hpp"

using namespace calyx;

UniformBranch::UniformBranch(std::vector<std::shared_ptr<Production>> choices)
    : _choices(choices)
{

}

UniformBranch::UniformBranch(const UniformBranch& old)
    : _choices(old._choices)
{

}

UniformBranch
UniformBranch::parse(std::vector<String_t> raw, const Registry& registry)
{
    std::vector<std::shared_ptr<Production>> choices;

    for (const auto& term : raw)
    {
        choices.push_back(std::make_shared<TemplateNode>(TemplateNode::parse(term, registry)));
    }

    return UniformBranch(choices);
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
    return Expansion(UNIFORM_BRANCH, std::make_unique<Expansion>(tail));
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