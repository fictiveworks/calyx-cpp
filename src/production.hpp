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

        virtual ~Production() = default;

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
        
    };

}
