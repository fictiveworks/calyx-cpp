#pragma once

#include "production.hpp"
#include "registry.hpp"

namespace calyx
{
    class UniformBranch: public ProductionBranch
    {
    public:
        Expansion evaluate(Options& options) const override;

        Expansion evaluateAt(int index, Options& options) const override;

        int length() const override;

    protected:
        ProductionBranch* deepcopyBranch() const override;

    private:
        std::vector<Production&> _choices;
        Registry& _registry;
    };
}