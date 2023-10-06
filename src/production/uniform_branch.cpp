#include "uniform_branch.h"
#include "../syntax/template_node.h"
#include <sstream>

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
UniformBranch::parse(const std::vector<String_t>& raw, const Registry& registry, ErrorHolder& errors)
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
    std::size_t index = options.randomInteger(_choices.size(), errors);

    if (errors.hasError())
    {
        return {};
    }
    
    return evaluateAt(index, registry, options, errors);
}

std::optional<Expansion>
UniformBranch::evaluateAt(
    std::size_t index,
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{

    if (index < 0 || index >= _choices.size())
    {
        std::ostringstream oss;
        oss << "Attempting to choose invalid expansion at index " << index;

        errors.setError(oss.str());
        return {};
    }
    
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
