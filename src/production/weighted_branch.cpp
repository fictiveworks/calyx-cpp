#include <numeric>

#include "weighted_branch.h"

#include "../syntax/template_node.h"

using namespace calyx;

WeightedBranch::WeightedProduction::WeightedProduction(double weight, std::shared_ptr<Production> production):
    _weight(weight),
    _production(production)
{
}

WeightedBranch::WeightedBranch(std::vector<WeightedProduction> productions):
    _productions(productions)
{
    _sumOfWeights = std::accumulate(
        productions.begin(), productions.end(), 0,
        [](int sum, const WeightedProduction& obj) { return sum + obj._weight; }
    );
}

WeightedBranch::WeightedBranch(const WeightedBranch& old):
    _productions(old._productions),
    _sumOfWeights(old._sumOfWeights)
{
}

std::optional<WeightedBranch>
WeightedBranch::parse(
    const std::map<String_t, double>& raw,
    const Registry& registry,
    ErrorHolder& errors
)
{
    std::vector<WeightedProduction> prods;
    for (const auto& entry : raw)
    {
        std::optional<TemplateNode> node = TemplateNode::parse(entry.first, registry, errors);
        if (!node || errors.hasError())
        {
            return {};
        }

        WeightedProduction prod(entry.second, std::make_shared<TemplateNode>(*node));
        prods.push_back(prod);
    }

    return WeightedBranch(prods);
}

std::optional<Expansion>
WeightedBranch::evaluate(
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{
    const WeightedProduction& prod = this->getRandomProduction(options);

    std::optional<Expansion> choice = prod._production->evaluate(registry, options, errors);

    if (!choice)
    {
        return {};
    }

    return Expansion(Exp::WEIGHTED_BRANCH, *choice);
}

const WeightedBranch::WeightedProduction&
WeightedBranch::getRandomProduction(Options& options) const
{
    double max = _sumOfWeights;
    double waterMark = options.randomReal<double>() * _sumOfWeights;

    for (const auto& wp : _productions)
    {
        max -= wp._weight;
        if (waterMark >= max)
        {
            return wp;
        }
    }

    // this should never happen, as the total weight should be greater than 0
    static const WeightedProduction default_result { 0.0, nullptr };
    return default_result;
}

std::optional<Expansion>
WeightedBranch::evaluateAt(
    std::size_t index,
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{
    std::optional<Expansion> tail = _productions[index]._production->evaluate(registry, options, errors);

    if (!tail)
    {
        return {};
    }

    return Expansion(Exp::WEIGHTED_BRANCH, *tail);
}

size_t
WeightedBranch::length() const
{
    return _productions.size();
}