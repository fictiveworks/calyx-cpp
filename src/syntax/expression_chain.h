#pragma once
#include "../production.h"

namespace calyx
{
    class ExpressionChain : public Production
    {
    public:

        ExpressionChain(std::vector<std::string> components);
        
        ~ExpressionChain() override = default;

        std::optional<Expansion> evaluate(Registry& registry, Options& options, ErrorHolder& errors) const override;

    private:
        std::vector<std::string> _components; 
    };
}