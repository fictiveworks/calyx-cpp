#pragma once

#include <memory>
#include <vector>
#include <optional>

#include "../production.h"
#include "../registry.h"

namespace calyx
{
    class WeightedBranch : public ProductionBranch
    {
        /**
         * @brief A type that represents a production with some weight,
         * stores a weight (double) and a shared ptr to a production
         *
         */
        struct WeightedProduction
        {
            double weight;
            const std::shared_ptr<Production> production;
        };

    public:

        WeightedBranch(const WeightedBranch& old);

        ~WeightedBranch() override = default;

        std::optional<Expansion> evaluate(
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const override;

        std::optional<Expansion> evaluateAt(
            std::size_t index,
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const override;

        size_t length() const override;

    private:

        std::vector<WeightedProduction> _productions;
        double _sumOfWeights;

        WeightedBranch(std::vector<WeightedProduction> productions);

        const WeightedProduction& getRandomProduction(Options& options) const;
    };
}