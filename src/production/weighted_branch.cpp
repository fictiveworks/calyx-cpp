#include <numeric>

#include "weighted_branch.hpp"

using namespace calyx;


WeightedBranch::WeightedBranch(std::vector<WeightedProduction> productions)
    : _productions(productions)
{

    _sumOfWeights = std::accumulate(
        productions.begin(), productions.end(), 0,
        [](int sum, const WeightedProduction& obj) { return sum + obj.weight; }
    );

}

WeightedBranch::WeightedBranch(const WeightedBranch& old)
    : _productions(old._productions),
    _sumOfWeights(old._sumOfWeights)
{
}

std::optional<Expansion>
WeightedBranch::evaluate(
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{
    const WeightedProduction& prod = this->getRandomProduction(options);

    std::optional<Expansion> choice = prod.production->evaluate(registry, options, errors);

    if (!choice) {
        return {};
    }

    return Expansion(Exp::WEIGHTED_BRANCH, *choice);
}

const WeightedBranch::WeightedProduction&
WeightedBranch::getRandomProduction(Options& options) const
{
    double max = _sumOfWeights;
    double waterMark = options.randDouble() * _sumOfWeights;

    for (const auto& wp : _productions) {
        max -= wp.weight;
        if (waterMark >= max) {
            return wp;
        }
    }

    // this should never happen, as the total weight should be greater than 0
    static const WeightedProduction default_result{ 0.0, nullptr };
    return default_result;
}

std::optional<Expansion>
WeightedBranch::evaluateAt(
    int index,
    Registry& registry,
    Options& options,
    ErrorHolder& errors
) const
{
    std::optional<Expansion> tail = _productions[index].production->evaluate(registry, options, errors);

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

