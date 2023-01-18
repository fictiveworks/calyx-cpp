#if defined(_WIN32)
#   define __export         __declspec(dllexport)
#elif defined(__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#   define __export         __attribute__((visibility("default")))
#else
#   define __export
#endif

#pragma once

#include <string>

#include "registry.hpp"

#include "string_converter.hpp"

class Result
{
public:
    Result(CalyxString txt);

    /**
     * [toString description]
     * @return [description]
     */
    CalyxString text();

private:
    CalyxString _text;
};

class Grammar
{
public:
    /**
     * [start description]
     * @param production  [description]
     */
    void start(CalyxString production);

    /**
     * [start description]
     * @param production  [description]
     */
    void start(std::vector<CalyxString> production);

    /**
     * [rule description]
     * @param term        [description]
     * @param production  [description]
     */
    void rule(CalyxString term, CalyxString production);

    /**
     * [start description]
     * @param term        [description]
     * @param production  [description]
     */
    void rule(std::vector<CalyxString> term, std::vector<CalyxString> production);

    /**
     * [generate description]
     * @return [description]
     */
    Result generate();

private:
    Registry& _registry;
};
