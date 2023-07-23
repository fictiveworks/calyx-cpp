#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <optional>

#include "options.hpp"
#include "production.hpp"
#include "rule.hpp"
#include "expansion.hpp"
#include "cycle.hpp"

namespace calyx
{
    class Registry
    {
    public:
        Registry();

        Registry(std::shared_ptr<Options> options);

        ~Registry() = default;

        Registry& operator=(const Registry& other);

        Options& getOptions() const; 

        void defineRule(String_t term, std::vector<String_t> production);

        std::optional<Expansion> evaluate(const String_t& startSymbol, ErrorHolder& errors);

        std::optional<Expansion> evaluate(const String_t& startSymbol, std::map<String_t, std::vector<String_t>> context, ErrorHolder& errors);

        std::optional<std::shared_ptr<Expansion>> memoizeExpansion(const String_t& symbol, ErrorHolder& errors);

        std::optional<Expansion> uniqueExpansion(const String_t& symbol, ErrorHolder& errors);

        std::optional<Rule> expand(const String_t& symbol, ErrorHolder& errors) const;

        void resetEvaluationContext();

    private:
        std::map<String_t, Rule> _rules;
        std::map<String_t, Rule> _context;
        std::map<String_t, std::shared_ptr<Expansion>> _memos;
        std::map<String_t, std::unique_ptr<Cycle>> _cycles;

        std::shared_ptr<Options> _options;
    };
}
