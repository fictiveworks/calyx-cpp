#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <optional>

#include "calyx/options.h"
#include "rule.h"
#include "expansion.h"
#include "cycle.h"
#include "calyx/filters.h"
#include "calyx/string_converter.h"

namespace calyx
{
    /**
     * @brief Central registry that tracks the rules of a grammar, as well as evaluation context like memoization, unique expansions,
     * and evaluation mode.
     *
     * The registry stores all of the rules of a Context-Free Grammar (CFG), as well as some additional context. Consider the following example,
     * which defines a grammar that can be used to generate strings that are palindromes over the alphabet {a, b}. 
     *
     * @code 
     * start -> a{start}a
     * start -> b{start}b
     * start -> {end}
     * end -> a
     * end -> b
     * @endcode 
     *
     * Each lower case a, b is an atomic string, and strings enclosed in {} are expansions. Each line of the form X -> Y, is a rule in the grammar.
     * The left hand side of the line is the name of the rule, and the right hand side is the Production it expands to.
     * 
     * When expanding a grammar that has multiple rules with the same name, one rule is picked at random. Users may specify that the rules are
     * picked with a uniform or weighted random distribution.
     *
     * By convention, evaluation begins from the "start" rule, however it is possible to start from any rule. The registry also provides for
     * the following additional facilities around grammars that are not typically available in CFGs:
     *
     * - Memoization (denoted by starting the template with @): Every call will always return the same expansion, useful for things like
     * names.
     * - Unique expansions (denoted by starting the template with $): Every call will return a different expansion. When all possible options have
     * been exhausted, it will then 'cycle' and produce the same options, but in a different order.
     * - Filters (denoted by dot-chaining the production): Transforms the output to some new string, like converting to all upper case characters
     * or returning the length of the string as a base-10 number. See calyx/filters.h for more information on the filters API, and how to add your
     * own filters to a registry and grammar.
     * 
     */
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
