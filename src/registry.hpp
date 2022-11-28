#pragma once

#include <string>
#include <vector>
#include <map>

#include "calyx.h"

class Registry;

class UniformBranch
{
public:
    UniformBranch(std::vector<std::string> production, Registry& registry)
    : _registry(registry)
    {
        _choices = production; // TODO: parse into syntax tree
    }

    std::string evaluate()
    {
        return _choices[0]; // TODO: evaluate syntax tree
    }

private:
    Registry& _registry;
    std::vector<std::string> _choices;
};

class Rule
{
public:
    Rule(std::string term, std::vector<std::string> production, Registry& registry)
    : _production(production, registry)
    {
        _term = term;
    }

    std::string evaluate()
    {
        return _production.evaluate();
    }

private:
    std::string _term;
    UniformBranch _production;
};

class Registry
{
public:
    void defineRule(std::string term, std::vector<std::string> production)
    {
        Rule rule = Rule(term, production, this);
        _rules[term] = rule;
    }

private:
    std::map<std::string, Rule> _rules;
};
