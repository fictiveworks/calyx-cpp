#pragma once

#include <memory>

#include "../production.hpp"
#include "../expansion.hpp"
#include "../string_converter.hpp"
#include "../options.hpp"
#include "../registry.hpp"

namespace calyx
{
    class ExpressionNode : public Production
    {
    public:
        ExpressionNode(const String_t reference);

        ~ExpressionNode() = default;

        static std::optional<ExpressionNode> parse(const String_t raw, const Registry& registry, ErrorHolder& errors);

        virtual std::optional<Expansion> evaluate(
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const override;

    private:
        const String_t _reference;

    };
}