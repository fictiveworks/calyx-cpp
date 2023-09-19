#if defined(_WIN32)
#define __export __declspec(dllexport)
#elif defined(__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#define __export __attribute__((visibility("default")))
#else
#define __export
#endif

#pragma once

#include <string>

#include "../registry.h"
#include "../options.h"
#include "string_converter.h"
#include "../errors.h"
#include "../expansion.h"

namespace calyx
{
    /**
     * \brief Class representing the result of an evaluation. Allows for access to the underlying expansion tree,
     * as well as the ability to flatten the tree into a string.
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
        const Expansion& getTree();

        /**
         * @brief Flattens the text into a new string, and returns it
         *
         * @param options Options for evaluating the text.
         * @return Returns a new string_t
         */
        String_t text(const Options& options);

    private:
        Expansion _tree;
    };

    class Grammar
    {
    public:

        Grammar();

        Grammar(Options opts);

        Grammar(bool strict = Options::DEFAULT_STRICT);

        Grammar(std::function<int()> rng, bool strict = Options::DEFAULT_STRICT);

        Grammar(unsigned int seed, bool strict = Options::DEFAULT_STRICT);

        ~Grammar();

        /**
         * [start description]
         * @param production  [description]
         */
        void start(String_t production);

        /**
         * [start description]
         * @param production  [description]
         */
        void start(std::vector<String_t> production);

        /**
         * [rule description]
         * @param term        [description]
         * @param production  [description]
         */
        void rule(String_t term, String_t production);

        /**
         * [start description]
         * @param term        [description]
         * @param production  [description]
         */
        void rule(std::vector<String_t> term, std::vector<String_t> production);

        /**
         * [generate description]
         * @return [description]
         */
        Result generate();

    private:
        Registry _registry;
    };
}
