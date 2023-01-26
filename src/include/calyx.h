#if defined(_WIN32)
#define __export __declspec(dllexport)
#elif defined(__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#define __export __attribute__((visibility("default")))
#else
#define __export
#endif

#pragma once

#include <string>

#include "registry.hpp"

#include "string_converter.hpp"

namespace calyx
{

    class Result
    {
    public:
        Result(String_t txt);

        /**
         * [toString description]
         * @return [description]
         */
        String_t text();

    private:
        String_t _text;
    };

    class Grammar
    {
    public:
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
        Registry &_registry;
    };
}
