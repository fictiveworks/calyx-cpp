#pragma once

#include "production.hpp"

namespace calyx
{
    class EmptyBranch : public ProductionBranch
    {
    public:

        Expansion evaluate(Options &options) const override;

        Expansion evaluateAt(int index, Options &options) const override;

        int length() const override;
    };
}