#pragma once

#include <vector>
#include <map>
#include <memory>

#include "expansion.hpp"
#include "production.hpp"
#include "string_converter.hpp"

namespace calyx
{

    class Registry;

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
        Rule(String_t term, std::unique_ptr<ProductionBranch> production);

        Rule(const Rule& other);

        Rule(Rule&& other) = default;

        ~Rule() = default;

        static Rule empty(String_t term);

        static Rule build(String_t term, std::vector<String_t> productions, const Registry& registry);

        Expansion evaluate(Options& options) const;

        Expansion evaluateAt(int index, Options& options) const;

        size_t length() const;

        Rule& operator=(const Rule& other);

        Rule& operator=(Rule&& other) = default;

    private:
        String_t _term;
        std::unique_ptr<ProductionBranch> _production;
    };
}