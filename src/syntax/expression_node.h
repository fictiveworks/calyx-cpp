#pragma once

#include <memory>

#include "../production.h"
#include "../expansion.h"
#include "../calyx/string_converter.h"
#include "../calyx/options.h"
#include "../registry.h"

namespace calyx
{
    class ExpressionNode : public Production
    {
    public:
        ExpressionNode(String_t reference);

        ~ExpressionNode() override = default;

        static bool isSigil(char character);

        static std::shared_ptr<Production> parse(const String_t& raw, const Registry& registry, ErrorHolder& errors);

        std::optional<Expansion> evaluate(
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const override;

    private:
        const String_t _reference;

    };
}