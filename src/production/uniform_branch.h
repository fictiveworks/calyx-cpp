#pragma once

#include <vector>
#include <memory>

#include "../production.h"
#include "../registry.h"
#include "../expansion.h"
#include "../include/string_converter.h"

namespace calyx
{
    class UniformBranch : public ProductionBranch
    {
    public:

        UniformBranch(std::vector<std::shared_ptr<Production>> choices);

        UniformBranch(const UniformBranch& old);

        ~UniformBranch() override = default;

        static std::optional<UniformBranch> parse(const std::vector<String_t>& raw, const Registry& registry, ErrorHolder& errors);

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
        std::vector<std::shared_ptr<Production>> _choices;
    };
}