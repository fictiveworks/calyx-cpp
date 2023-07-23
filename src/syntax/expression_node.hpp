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
        ExpressionNode(const String_t reference, const Registry& registry);

        ~ExpressionNode() = default;

        static std::optional<std::shared_ptr<ExpressionNode>> parse(const String_t raw, const Registry& registry, ErrorHolder& errors);

        virtual std::optional<Expansion> evaluate(Options& options, ErrorHolder& errors) const override;

    protected:

        virtual Production* deepcopy() const;

    private:
        const String_t _reference;

        // TODO: figure out safer way to reference registries than this 
        const Registry& _registry;

    };
}