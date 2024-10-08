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
     * and filters.
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

        /**
         * @brief Defines a rule with uniform expansions
         * 
         * @param term Name of the rule 
         * @param production Uniform expansions
         * @param errors Error holder, if an error occurs then the rule is not added to the registry
         */
        void defineRule(String_t term, const std::vector<String_t>& production, ErrorHolder& errors);

        /**
         * @brief Defines a rule with weighted expansions
         * 
         * @param term Name of the rule 
         * @param productions Map of expansions to weights
         * @param errors Error holder, if an error occurs then the rule is not added to the registry
         */
        void defineRule(String_t term, const std::map<String_t, double>& productions, ErrorHolder& errors);

        /**
         * @brief Adds a filter to the registry 
         * @param name The name of the filter 
         * @param filter The filter function 
         */
        void addFilter(String_t name, filters::Filter_t filter);

        /**
         * @brief Gets the filter for the given name, if it exists in the registry 
         * @param name The name of the filter to get
         * @return Returns an optional with the filter function, if found. Otherwise returns an empty optional.
         */
        std::optional<const filters::Filter_t> getFilter(const String_t& name) const;

        /**
         * @brief Evaluates the productions of the registry, starting from the given startSymbol
         * 
         * @param startSymbol The start point of the evaluation
         * @param errors Error holder that records any errors that may arise from evaluation.  
         * @return Returns an optional that contains the evaluation tree if any only if no errors occurred
         */
        std::optional<Expansion> evaluate(const String_t& startSymbol, ErrorHolder& errors);
        
        /**
         * @brief Evaluates the productions of the registry, starting from the given startSymbol. Uses the fallback
         * dynamic context if attempting to expand to a rule not known to the registry.
         * 
         * @param startSymbol The start point of the evaluation
         * @param context The dynamic context. Expands as a uniform branch.
         * @param errors Error holder that records any errors that may arise from evaluation.  
         * @return Returns an optional that contains the evaluation tree if any only if no errors occurred
         */
        std::optional<Expansion> evaluate(
            const String_t& startSymbol,
            const std::map<String_t, std::vector<String_t>>& context,
            ErrorHolder& errors
        );

        /**
         * @brief Expands the symbol, but if it has already been expanded during this evaluation then it returns
         * the expansion it expanded to the first time. 
         * 
         * @param symbol The symbol to expand
         * @param errors Error holder that records errors that occur during evaluation
         * @return Returns a shared pointer to the expansion tree from the evaluation. This pointer is null if and only
         * if there are no errors.
         */
        std::shared_ptr<Expansion> memoizeExpansion(const String_t& symbol, ErrorHolder& errors);

        /**
         * @brief Expands the symbol, but returns a different expansion each time it is called during this evaluation.
         * 
         * If all options are exhausted, the unique expansions will cycle and produce the same results that they already
         * have, but in a new random order.
         * 
         * @param symbol The symbol to expand
         * @param errors Error holder that records errors that occur during evaluation
         * @return Returns an optional with the expansion tree from the evaluation. This optional is empty if and only
         * if there are no errors.
         */
        std::optional<Expansion> uniqueExpansion(const String_t& symbol, ErrorHolder& errors);

        /**
         * @brief Gets the rule associated with the given symbol.
         *
         * An error will occur if operating with options in 'strict' mode and the symbol is not a valid rule in either
         * the static rules or dynamic context.
         * 
         * @param symbol The symbol to expand
         * @param errors Error holder that records errors that occur during evaluation
         * @return Returns an optional with the rule from the evaluation. This optional is empty if and only
         * if there are no errors.
         */
        std::shared_ptr<Rule> expand(const String_t& symbol, ErrorHolder& errors) const;

        /**
         * @brief Resets the evaluation-specific context variables. Namely, the context, memos, and cycles.
         */
        void resetEvaluationContext();

    private:
        std::map<String_t, std::shared_ptr<Rule>> _rules;

        // evaluation specific context variables
        std::map<String_t, std::shared_ptr<Rule>> _context;
        std::map<String_t, std::shared_ptr<Expansion>> _memos;
        std::map<String_t, std::shared_ptr<Cycle>> _cycles;

        std::map<String_t, filters::Filter_t> _filters;
        std::shared_ptr<Options> _options;
    };
}
