#pragma once

#include <vector>
#include <map>
#include <memory>
#include <optional>

#include "expansion.hpp"
#include "include/string_converter.hpp"
#include "production.hpp"

namespace calyx
{

    class Rule
    {
    public:

        Rule();

        /**
         * @brief Construct a new Rule object with a term and production.
         * 
         * @param term 
         * @param production 
         */
        Rule(String_t term, std::shared_ptr<ProductionBranch> production);

        Rule(const Rule& other);

        Rule(Rule&& other) = default;

        ~Rule() = default;

        static Rule empty(String_t term);

        static std::optional<Rule> build(String_t term, std::vector<String_t> productions, const Registry& registry, ErrorHolder& errors);

        std::optional<Expansion> evaluate(
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const;

        std::optional<Expansion> evaluateAt(
            int index,
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const;

        size_t length() const;

        Rule& operator=(const Rule& other);

        Rule& operator=(Rule&& other) = default;

    private:
        String_t _term;
        std::shared_ptr<ProductionBranch> _production;
    };
}