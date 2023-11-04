#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <optional>

#include "options.h"
#include "rule.h"
#include "expansion.h"
#include "cycle.h"
#include "calyx/filters.h"
#include "calyx/string_converter.h"

namespace calyx
{
    
    class Registry
    {
    public:
        Registry();

        Registry(Options options);

        Registry(std::shared_ptr<Options> options);

        ~Registry() = default;

        Registry(Registry&& old) noexcept;

        Registry(const Registry& other) = delete;
        
        Registry& operator=(const Registry& other) = delete;

        Registry& operator=(Registry&& other) noexcept;

        Options& getOptions() const; 

        void defineRule(String_t term, const std::vector<String_t>& production, ErrorHolder& errors);

        void addFilter(String_t name, filters::Filter_t filter);

        std::optional<const filters::Filter_t> getFilter(const String_t& name) const;
        
        void defineRule(String_t term, const std::map<String_t, double>& productions, ErrorHolder& errors);

        std::optional<Expansion> evaluate(const String_t& startSymbol, ErrorHolder& errors);

        std::optional<Expansion> evaluate(const String_t& startSymbol, std::map<String_t, std::vector<String_t>> context, ErrorHolder& errors);

        std::shared_ptr<Expansion> memoizeExpansion(const String_t& symbol, ErrorHolder& errors);

        std::optional<Expansion> uniqueExpansion(const String_t& symbol, ErrorHolder& errors);

        std::shared_ptr<Rule> expand(const String_t& symbol, ErrorHolder& errors) const;

        void resetEvaluationContext();

    private:
        std::map<String_t, std::shared_ptr<Rule>> _rules;
        std::map<String_t, std::shared_ptr<Rule>> _context;
        std::map<String_t, std::shared_ptr<Expansion>> _memos;
        std::map<String_t, std::shared_ptr<Cycle>> _cycles;

        std::map<String_t, filters::Filter_t> _filters;
        std::shared_ptr<Options> _options;
    };
}
