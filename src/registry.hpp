#pragma once

#include <string>
#include <vector>
#include <map>

#include "calyx.h"
#include "options.hpp"

namespace calyx
{

    class Rule;

    class Registry
    {
    public:
        Registry();

        Registry(Options options);

        ~Registry();

        Registry &operator=(const Registry &other);

        void defineRule(String_t term, std::vector<String_t> production);

    private:
        std::map<String_t, Rule> _rules;
        Options *_options;
    };

    class UniformBranch
    {
    public:
        UniformBranch(std::vector<String_t> production, Registry &registry)
            : _registry(registry)
        {
            _choices = production; // TODO: parse into syntax tree
        }

        String_t evaluate()
        {
            return _choices[0]; // TODO: evaluate syntax tree
        }

        UniformBranch &operator=(const UniformBranch &other)
        {
            _registry = other._registry;
            _choices = other._choices;

            return *this;
        }

    private:
        Registry &_registry;
        std::vector<String_t> _choices;
    };

    class Rule
    {
    public:
        Rule(String_t term, std::vector<String_t> production, Registry &registry)
            : _production(production, registry)
        {
            _term = term;
        }

        String_t evaluate()
        {
            return _production.evaluate();
        }

        Rule &operator=(const Rule &other)
        {
            _term = other._term;
            _production = other._production;

            return *this;
        }

    private:
        String_t _term;
        UniformBranch _production;
    };
}
