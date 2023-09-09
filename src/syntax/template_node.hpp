#pragma once

#include "../production.hpp"
#include "../expansion.hpp"
#include "../include/string_converter.hpp"
#include "../options.hpp"
#include "../registry.hpp"

namespace calyx
{
    class TemplateNode : public Production
    {
    public:

        TemplateNode(std::vector<std::shared_ptr<Production>> concatNodes);

        ~TemplateNode() = default;

        virtual std::optional<Expansion> evaluate(
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const override;

        static std::optional<TemplateNode> parse(const String_t& raw, const Registry& registry, ErrorHolder& errors);

        /**
         * @brief Fragments a std string into a vector of strings tokens. For example, the string {@code "{prod} words words {prod}"}
         * will be fragmented into {@code ["{prod}", " words words ", "{prod}"]}.
         * 
         * This method is used by {@link #parse} and is exposed here in the public API for testing purposes.
         * 
         * @param raw The raw string to fragment
         * @return std::vector<std::string> A fragmented list of tokens and raw strings
         */
        static std::vector<std::string> fragmentString(const std::string& raw);

    private:
        std::vector<std::shared_ptr<Production>> _concatNodes;
    };
}