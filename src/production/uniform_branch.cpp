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

std::optional<UniformBranch>
UniformBranch::parse(std::vector<String_t> raw, const Registry& registry, ErrorHolder& errors)
{
    std::vector<std::shared_ptr<Production>> choices;

    for (const auto& term : raw)
    {
        std::optional<TemplateNode> templateNode = TemplateNode::parse(term, registry, errors);

        if (!templateNode)
        {
            return {};
        }

        choices.push_back(std::make_shared<TemplateNode>(*templateNode));
    }

    return UniformBranch(choices);
}


std::optional<Expansion>
UniformBranch::evaluate(
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{
    int index = options.randInt(_choices.size());
    return evaluateAt(index, registry, options, errors);
}

std::optional<Expansion>
UniformBranch::evaluateAt(
    int index,
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{
    std::shared_ptr<Production> choice = _choices.at(index);
    std::optional<Expansion> tail = choice->evaluate(registry, options, errors);

    if (!tail)
    {
        return {};
    }

    return Expansion(Exp::UNIFORM_BRANCH, *tail);
}

size_t UniformBranch::length() const
{
    return _choices.size();
}
