#pragma once

#include <memory>
#include <optional>

#include "expansion.hpp"
#include "options.hpp"

namespace calyx
{

    class Registry;

    class Production
    {
    public:
        virtual std::optional<Expansion> evaluate(
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const = 0;

        std::unique_ptr<Production> clone() const
        {
            return std::unique_ptr<Production>(deepcopy());
        }

    protected:

        virtual Production* deepcopy() const = 0;

    };

    class ProductionBranch: public Production
    {
    public:
        virtual std::optional<Expansion> evaluateAt(
            int index,
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const = 0;

        virtual size_t length() const = 0;

        std::unique_ptr<ProductionBranch> cloneBranch() const
        {
            return std::unique_ptr<ProductionBranch>(deepcopyBranch());
        }

    protected:

        virtual ProductionBranch* deepcopyBranch() const = 0;

        Production* deepcopy() const override
        {
            return deepcopyBranch();
        }
    };

}
