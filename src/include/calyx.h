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

class Result
{
public:
    Result(std::string txt);

    /**
     * [toString description]
     * @return [description]
     */
    std::string text();

private:
    std::string _text;
};

class Grammar
{
public:
    /**
     * [start description]
     * @param production  [description]
     */
    void start(std::string production);

    /**
     * [start description]
     * @param production  [description]
     */
    void start(std::vector<std::string> production);

    /**
     * [rule description]
     * @param term        [description]
     * @param production  [description]
     */
    void rule(std::string term, std::string production);

    /**
     * [start description]
     * @param term        [description]
     * @param production  [description]
     */
    void rule(std::vector<std::string> term, std::vector<std::string> production);

    /**
     * [generate description]
     * @return [description]
     */
    Result generate();

private:
    Registry& _registry;
};
