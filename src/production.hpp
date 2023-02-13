#pragma once

#include "expansion.hpp"
#include "options.hpp"

namespace calyx
{

    class Production
    {
    public:
        virtual Expansion evaluate(Options &options) const = 0;
    };

    class ProductionBranch : public Production
    {
    public:
        virtual Expansion evaluateAt(int index, Options &options) const = 0;

        virtual int length() const = 0;
    };

}
