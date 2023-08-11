#pragma once

#include <optional>
#include "../production.hpp"

namespace calyx
{
    class EmptyBranch : public ProductionBranch
    {
    public:

        EmptyBranch() = default;

        EmptyBranch(const EmptyBranch& old);

        ~EmptyBranch() = default;

        std::optional<Expansion> evaluate(
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const override;

        std::optional<Expansion> evaluateAt(
            int index,
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const override;

        size_t length() const override;

    };
}