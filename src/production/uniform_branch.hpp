#pragma once

#include <vector>
#include <memory>

#include "../production.hpp"
#include "../registry.hpp"
#include "../expansion.hpp"
#include "../string_converter.hpp"

namespace calyx
{
    class UniformBranch: public ProductionBranch
    {
    public:

        UniformBranch(std::vector<std::shared_ptr<Production>> choices);

        UniformBranch(const UniformBranch& old);

        ~UniformBranch() = default;

        static UniformBranch parse(std::vector<String_t> raw, const Registry& registry);
        
        Expansion evaluate(Options& options) const override;

        Expansion evaluateAt(int index, Options& options) const override;

        inline size_t length() const override;

    protected:
        ProductionBranch* deepcopyBranch() const override;

    private:
        std::vector<std::shared_ptr<Production>> _choices;
    };
}