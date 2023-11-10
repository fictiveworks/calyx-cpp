#pragma once

#include <vector>
#include <map>
#include <memory>
#include <optional>

#include "expansion.h"
#include "calyx/string_converter.h"
#include "production.h"

namespace calyx
{
    /**
     * @brief A mapping of a named rule to a production.  
     */
    class Rule
    {
    public:
        Rule() = default;

        /**
         * @brief Construct a new Rule object with a term and production.
         * 
         * @param term 
         * @param production 
         */
        Rule(String_t term, std::shared_ptr<ProductionBranch> production);

        Rule(const Rule& other) = default;

        Rule(Rule&& other) = default;

        ~Rule() = default;

        static Rule empty(String_t term);

        static std::optional<Rule> build(
            String_t term,
            const std::vector<String_t>& productions,
            const Registry& registry,
            ErrorHolder& errors
        );

        static std::optional<Rule> build(
            String_t term,
            const std::map<String_t, double>& productions,
            const Registry& registry,
            ErrorHolder& errors
        );

        std::optional<Expansion> evaluate(
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const;

        std::optional<Expansion> evaluateAt(
            std::size_t index,
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