#pragma once

#include <memory>
#include <optional>

#include "expansion.h"
#include "calyx/options.h"

namespace calyx
{
    class Registry;

    /**
     * @brief Base interface for all productions. A production is an individual rule in a grammar, and can be
     * evaluated to produce an expansion tree. 
     */
    class Production
    {
    public:
        /**
         * @brief Evaluates the production. Returns a randomly generated expansion tree if no errors occur. 
         * 
         * @param registry The registry that called for this expansion. Used for things that require some external
         * context, like memoization and unique expansions. 
         * @param options Options for evaluation, used primarily for its random access. 
         * @param errors Error holder for errors that may occur in the expansion. 
         * @return Returns an optional containing the randomly generated expansion tree, if and only if no error
         * has occured. 
         */
        virtual std::optional<Expansion> evaluate(
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const = 0;

        virtual ~Production() = default;
    };

    /**
     * @brief Contains several productions and picks one randomly according to some rule. For example, randomly pick a child production
     * with uniform probability. 
     */
    class ProductionBranch : public Production
    {
    public:
        /**
         * @brief Picks and expands a production at some index, according to its random distribution rule.
         * 
         * @param registry The registry that called for this expansion. Used for things that require some external
         * context, like memoization and unique expansions. 
         * @param options Options for evaluation, used primarily for its random access. 
         * @param errors Error holder for errors that may occur in the expansion. 
         * @return Returns an optional containing the randomly generated expansion tree, if and only if no error
         * has occured. 
         */
        std::optional<Expansion> evaluate(
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const override = 0;

        /**
         * @brief Expands the child production at the specified index.
         * 
         * @param index The index of the child production to expand. 
         * @param registry The registry that called for this expansion. Used for things that require some external
         * context, like memoization and unique expansions. 
         * @param options Options for evaluation, used primarily for its random access. 
         * @param errors Error holder for errors that may occur in the expansion. 
         * @return Returns an optional containing the randomly generated expansion tree, if and only if no error
         * has occured. 
         */
        virtual std::optional<Expansion> evaluateAt(
            std::size_t index,
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const = 0;

        virtual std::size_t length() const = 0;
    };
}