#pragma once

#include "../production.hpp"
#include "../expansion.hpp"
#include "../string_converter.hpp"
#include "../options.hpp"
#include "../registry.hpp"

namespace calyx
{
    class TemplateNode : Production
    {
    public:

        TemplateNode(std::vector<std::shared_ptr<Production>> concatNodes);

        ~TemplateNode() = default;

        virtual Expansion evaluate(Options& options) const override;

        static TemplateNode parse(const String_t& raw, const Registry& registry);

    protected:
        
        virtual Production* deepcopy() const override;

    private:
        std::vector<std::shared_ptr<Production>> _concatNodes;
    };
}