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

        static std::optional<UniformBranch> parse(std::vector<String_t> raw, const Registry& registry, ErrorHolder& errors);
        
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

    protected:
        ProductionBranch* deepcopyBranch() const override;

    private:
        std::vector<std::shared_ptr<Production>> _choices;
    };
}