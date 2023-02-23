#pragma once

#include <memory>

#include "expansion.hpp"
#include "options.hpp"

namespace calyx
{

    class Production
    {
    public:
        virtual Expansion evaluate(Options& options) const = 0;

        auto clone() const
        {
            return std::unique_ptr<Production>(deepcopy());
        }

    protected:

        virtual Production* deepcopy() const = 0;

    };

    class ProductionBranch: public Production
    {
    public:
        virtual Expansion evaluateAt(int index, Options& options) const = 0;

        virtual inline size_t length() const = 0;

        auto cloneBranch() const
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
