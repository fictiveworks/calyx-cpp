#pragma once

#include <string>
#include <vector>
#include <map>

#include "calyx.h"
#include "options.hpp"
#include "production.hpp"
#include "rule.hpp"
#include "expansion.hpp"

namespace calyx
{
    class Registry
    {
    public:
        Registry();

        Registry(Options options);

        ~Registry();

        Registry& operator=(const Registry& other);

        inline Options& getOptions() const; 

        void defineRule(String_t term, std::vector<String_t> production);

        Expansion evaluate(const String_t& startSymbol, ErrorHolder& errors);

        Expansion evaluate(const String_t& startSymbol, std::map<String_t, std::vector<String_t>> context, ErrorHolder& errors);

        Expansion memoizeExpansion(const String_t& symbol, ErrorHolder& errors);

        Rule expand(const String_t& symbol, ErrorHolder& errors) const;

        void resetEvaluationContext();

    private:
        std::map<String_t, Rule> _rules;
        std::map<String_t, Rule> _context;
        std::map<String_t, Expansion> _memos;

        Options* _options;
    };
}
