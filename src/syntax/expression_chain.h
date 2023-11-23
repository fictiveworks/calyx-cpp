#pragma once
#include "../production.h"

namespace calyx
{
    class ExpressionChain : public Production
    {
    public:

        static std::shared_ptr<Production> parse(
            const std::vector<std::string>& components,
            const Registry& registry,
            ErrorHolder& errors
        );
        
        ExpressionChain(String_t ruleName, std::vector<std::string> components);
        
        ~ExpressionChain() override = default;

        std::optional<Expansion> evaluate(Registry& registry, Options& options, ErrorHolder& errors) const override;

    private:
        String_t _ruleName;
        std::vector<String_t> _components; 
    };
}