#include <numeric>

#include "weighted_branch.hpp"

using namespace calyx;


WeightedBranch::WeightedBranch(std::vector<WeightedProduction> productions, Registry& registry)
    : _productions(productions),
    _registry(registry)
{

    _sumOfWeights = std::accumulate(
        productions.begin(), productions.end(), 0,
        [](int sum, const WeightedProduction& obj) { return sum + obj.weight; }
    );

}

WeightedBranch::WeightedBranch(const WeightedBranch& old)
    : _productions(old._productions),
    _registry(old._registry),
    _sumOfWeights(old._sumOfWeights)
{
}

Expansion
WeightedBranch::evaluate(Options& options) const
{
    const WeightedProduction& prod = this->getRandomProduction(options);

    return Expansion(WEIGHTED_BRANCH, std::make_unique<Expansion>(prod.production->evaluate(options)));
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
    static const WeightedProduction default_result {0.0, nullptr};
    return default_result;
}

Expansion
WeightedBranch::evaluateAt(int index, Options& options) const
{
    Expansion tail = _productions[index].production->evaluate(options);

    return Expansion(WEIGHTED_BRANCH, std::make_unique<Expansion>(tail));
}

size_t
WeightedBranch::length() const
{
    return _productions.size();
}

ProductionBranch*
WeightedBranch::deepcopyBranch() const
{
    return new WeightedBranch(*this);
}