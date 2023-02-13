#pragma once

#include <vector>
#include <map>

#include "expansion.hpp"
#include "production.hpp"
#include "string_converter.hpp"

namespace calyx
{

    class Rule
    {
    public:
        Rule(String_t term, ProductionBranch &production);

        static Rule build(String_t term, std::vector<String_t> productions, Registry &registry);

        Expansion evaluate(Options &options) const;

        Expansion evaluateAt(int index, Options &options) const;

        Rule &operator=(const Rule &other);

    private:
        String_t _term;
        ProductionBranch &_production;
    };
}