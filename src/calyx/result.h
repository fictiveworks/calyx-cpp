#pragma once

#include <string>

#include "filters.h"
#include "../registry.h"
#include "../options.h"
#include "string_converter.h"

namespace calyx
{
    /**
     * @brief Class representing the result of a random generation of text from a grammar. Allows for access to the underlying
     * expansion tree, as well as the ability to flatten the tree into a string.
     */
    class Result
    {
    public:
        /**
         * @brief Constructs the Result out of an expansion tree.
         * @param tree The expansion tree of the result.
         */
        Result(Expansion tree);

        /**
         * @brief Get the expansion tree of the result
         * 
         * @return Returns a reference to the expansion tree of this result  
         */
        const Expansion& getTree() const;

        /**
         * @brief Flattens the text into a new string, and returns it
         *
         * @param options Options for evaluating the text.
         * @return Returns a new string_t
         */
        String_t getText(const Options& options) const;

    private:
        Expansion _tree;
    };
}