#pragma once

#include "../production.hpp"

namespace calyx
{
    class EmptyBranch: public ProductionBranch
    {
    public:

        EmptyBranch() = default;

        EmptyBranch(const EmptyBranch& old);

        ~EmptyBranch() = default;

        Expansion evaluate(Options& options) const override;

        Expansion evaluateAt(int index, Options& options) const override;

        size_t length() const override;

    protected:
        ProductionBranch* deepcopyBranch() const override;
    };
}