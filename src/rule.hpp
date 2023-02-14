#pragma once

#include <vector>
#include <map>
#include <memory>

#include "expansion.hpp"
#include "production.hpp"
#include "string_converter.hpp"

namespace calyx
{

    class Rule
    {
    public:

        Rule();

        Rule(String_t term, ProductionBranch& production);

        Rule(const Rule& other);

        Rule(Rule&& other) = default;

        ~Rule();

        static Rule empty(String_t term);

        static Rule build(String_t term, std::vector<String_t> productions, Registry& registry);

        Expansion evaluate(Options& options) const;

        Expansion evaluateAt(int index, Options& options) const;

        int length() const;

        Rule& operator=(const Rule& other);

        Rule& operator=(Rule&& other) = default;

    private:
        String_t _term;
        std::unique_ptr<ProductionBranch> _production;
    };
}