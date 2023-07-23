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

        static std::shared_ptr<ExpressionNode> parse(const String_t raw, const Registry& registry);

        virtual Expansion evaluate(Options& options) const override;

    protected:

        virtual Production* deepcopy() const;

    private:
        const String_t _reference;

        // TODO: figure out safer way to reference registries than this 
        const Registry& _registry;

    };
}