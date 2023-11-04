#if defined(_WIN32)
#define __export __declspec(dllexport)
#elif defined(__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#define __export __attribute__((visibility("default")))
#else
#define __export
#endif

#pragma once

#include <string>

#include "filters.h"
#include "../registry.h"
#include "options.h"
#include "string_converter.h"
#include "errors.h"
#include "result.h"

namespace calyx
{

    /**
     * @brief Defines a grammar in calyx. Grammars are a set of mutually-recursive rules that are evaluated randomly in order to
     * produce a text. Defines various constructors for configuring the generator, and methods for generating text.
     *
     * Grammars may be strict or non-strict. Strict Grammars generate an error when attempting to evaluate unknown rules.
     * Non-strict Grammars will evaluate unknown rules to an empty string.
     */
    class Grammar
    {
    public:
        /**
         * @brief Constructs a grammar with default options
         */
        Grammar() noexcept;

        /**
         * @brief Constructs a grammar with pre-set options
         * @param opts The options of the grammar
         */
        explicit Grammar(Options opts) noexcept;

        /**
         * @brief Constructs a grammar with a configured strict mode.
         * 
         * @param strict Whether or not to make this grammar strict. 
         */
        explicit Grammar(bool strict) noexcept;

        /**
         * @brief Constructs a grammar with a configured random number generator (rng) and strict mode. The random number generator
         * can be configured as you wish.
         *
         * @param rng Mersenne-twister based random number generator.
         * @param strict Whether or not to make this grammar strict.
         */
        explicit Grammar(std::mt19937 rng, bool strict) noexcept;

        /**
         * @brief Constructs a grammar with a configured random seed and strict mode.
         *
         * @param seed The seed for the random number generator.
         * @param strict Whether or not to make this grammar strict.
         */
        explicit Grammar(unsigned long seed, bool strict) noexcept;

        /**
         * @brief Constructs a grammar with a configured strict mode and initialization callback.
         *
         * The initialization callback is invoked after all of this grammar's members are defined, and can be used to add rules into the
         * grammar from the constructor. For example:
         *
         * @code
         * Grammar grammar(
         *     [](Grammar& g) -> void {
         *         g.rule("start", { "Hello World!" });
         *     }
         * );
         * @endcode 
         *
         * @param initializeCallback The initialization callback
         * @param strict Whether or not to make this grammar strict.
         */
        explicit Grammar(const std::function<void(Grammar&)>& initializeCallback, bool strict) noexcept;

        /**
         * @brief Constructs a grammar with a configured strict mode, random number generator, and initialization callback.
         *
         * @see Grammar(const std::function<void(Grammar&)>&, bool)
         * 
         * @param initializeCallback The initialization callback
         * @param rng Mersenne-twister based random number generator.
         * @param strict Whether or not to make this grammar strict. 
         */
        explicit Grammar(
            const std::function<void(Grammar&)>& initializeCallback,
            std::mt19937 rng,
            bool strict
        ) noexcept;

        /**
         * @brief Constructs a grammar with a configured strict mode, random seed, and initialization callback.
         *
         * @see Grammar(const std::function<void(Grammar&)>&, bool)
         * 
         * @param initializeCallback The initialization callback
         * @param seed The seed for the random number generator.
         * @param strict Whether or not to make this grammar strict.
         */
        explicit Grammar(
            const std::function<void(Grammar&)>& initializeCallback,
            unsigned long seed,
            bool strict
        ) noexcept;

        Grammar(Grammar&& old) noexcept;

        Grammar(const Grammar& other) = delete;

        ~Grammar() = default;

        Grammar& operator=(Grammar&& old) noexcept;

        Grammar& operator=(const Grammar& old) = delete;

        /**
         * @brief Defines a new single-value start rule for the grammar.
         *
         * If an error occurs while parsing this production, then the rule will NOT be added to
         * the grammar.
         * 
         * @param production The resulting production of the start rule.
         * @param errors Error handler containing errors messages that may arise when parsing this rule. 
         */
        void start(String_t production, ErrorHolder& errors) noexcept;

        /**
         * @brief Defines a uniform probability distribution for the start rule of the grammar.
         *
         * If an error occurs while parsing this production, then the rule will NOT be added to
         * the grammar.
         * 
         * @param production A uniform branch of productions for this rule.
         * @param errors Error handler containing errors messages that may arise when parsing this rule. 
         */
        void start(const std::vector<String_t>& production, ErrorHolder& errors) noexcept;

        /**
         * @brief Defines a weighted probability distribution for the start rule for the grammar.
         *
         * If an error occurs while parsing this production, then the rule will NOT be added to
         * the grammar.
         * 
         * @param productions A custom weighted branch of productions for this rule.
         * @param errors Error handler containing errors messages that may arise when parsing this rule. 
         */
        void start(const std::map<String_t, double>& productions, ErrorHolder& errors) noexcept;

        /**
         * @brief Defines a new single-value rule for the grammar. In this case, there is only one possible choice for the result when
         * the term is evaluated.
         *
         * If an error occurs while parsing this rule, then the rule will NOT be added to
         * the grammar.
         *
         * @param term The name of the new rule
         * @param production The resulting production of this rule.
         * @param errors Error handler containing errors messages that may arise when parsing this rule. 
         */
        void rule(String_t term, String_t production, ErrorHolder& errors) noexcept;

        /**
         * @brief Defines a new rule for the grammar. In this case, there are several possible choices for the result.
         * When the term is evaluated, one of these productions is chosen at random with a uniform distribution.
         *
         * If an error occurs while parsing this production, then the rule will NOT be added to
         * the grammar.
         *
         * @param term The name of the new rule
         * @param production A uniform branch of productions for this rule.
         * @param errors Error handler containing errors messages that may arise when parsing this rule. 
         */
        void rule(String_t term, const std::vector<String_t>& production, ErrorHolder& errors) noexcept;

        /**
         * @brief Defines a weighted probability distribution rule for the grammar.
         *
         * If an error occurs while parsing this production, then the rule will NOT be added to
         * the grammar.
         * 
         * @param term The name of the new rule
         * @param productions A custom weighted branch of productions for this rule.
         * @param errors Error handler containing errors messages that may arise when parsing this rule. 
         */
        void rule(String_t term, const std::map<String_t, double>& productions, ErrorHolder& errors) noexcept;

        /**
         * @brief Randomly generates some text using this Grammar's rules, starting from the "start" rule.
         * 
         * @param errors Error handler containing errors messages that may arise when generating text.
         * @return Returns an optional that contains the randomly generated result. This optional contains a value if and only if
         * errors does not have an error.
         */
        [[nodiscard]] std::optional<Result> generate(ErrorHolder& errors) noexcept;

        /**
         * @brief Randomly generates some text using this Grammar's rules, starting from a given rule.
         *
         * @param start The name of the rule to start generating from.
         * @param errors Error handler containing errors messages that may arise when generating text.
         * @return Returns an optional that contains the randomly generated result. This optional contains a value if and only if
         * errors does not have an error.
         */
        [[nodiscard]] std::optional<Result> generate(const String_t& start, ErrorHolder& errors) noexcept;

        /**
         * @brief Adds filters to the grammar, as a map of filter names to filter functions.
         *
         * Filters are functions that can transform the output of an evaluated production. For example, to convert the string to
         * upper case. See filters.h for more details.
         * 
         * @param filters The filters to add to the grammar. The values of the map are copied into the grammar, and so the original
         * map may be discarded by the caller.
         */
        void filters(const std::map<String_t, filters::Filter_t>& filters) noexcept;

        /**
        * @brief Add a filters to the grammar.
         *
         * Filters are functions that can transform the output of an evaluated production. For example, to convert the string to
         * upper case. See filters.h for more details.
         * 
         * @param filterName The name of the filter to be added
         * @param filter The filter function itself
         */
        void filter(String_t filterName, filters::Filter_t filter) noexcept;

        Options& getOptions() const noexcept;

    private:
        Registry _registry;
    };
}