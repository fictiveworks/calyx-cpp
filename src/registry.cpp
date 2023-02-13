#include "registry.hpp"

using namespace calyx;

Registry::Registry() : Registry(Options())
{
}

Registry::Registry(Options options)
    : _options(new Options(options)),
      _rules(std::map<String_t, Rule>())
{
}

Registry::~Registry()
{
    delete _options;
}

Registry &
Registry::operator=(const Registry &other)
{
    _rules = other._rules;
    delete _options;
    _options = new Options(other._options);

    return *this;
}

void Registry::defineRule(String_t term, std::vector<String_t> production)
{
    Rule rule = Rule::build(term, production, *this);

    _rules[term] = rule;
}