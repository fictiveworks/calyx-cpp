#pragma once

#include <memory>
#include <optional>

#include "expansion.h"
#include "options.h"

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
            std::size_t index,
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const = 0;

        virtual std::size_t length() const = 0;
        
    };

}
