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

Options&
Registry::getOptions() const
{
    return *_options;
}

Registry&
Registry::operator=(const Registry& other)
{
    _rules = other._rules;
    delete _options;
    _options = new Options(other._options);

    return *this;
}

void
Registry::defineRule(String_t term, std::vector<String_t> production)
{
    Rule rule = Rule::build(term, production, *this);

    _rules[term] = rule;
}

Expansion
Registry::evaluate(const String_t& startSymbol, ErrorHolder& errors)
{
    resetEvaluationContext();

    Rule rule;
    expand(startSymbol, rule, errors);

    if (errors.hasError()) {
        return Expansion(ERROR, _options->_converter.fromString(""));
    }

    Expansion root = Expansion(RESULT, std::make_shared<Expansion>(rule.evaluate(getOptions())));

    return root;
}

void
Registry::expand(const String_t& symbol, Rule& out, ErrorHolder& errors) const
{
    if (_rules.contains(symbol))
    {
        out = _rules.at(symbol);
    }
    else if (_context.contains(symbol))
    {
        out = _context.at(symbol);
    }
    else
    {
        if (_options->_strict) {
            // strict - do not allow empty rules
            // no early return since the empty rule will be returned anyway
            errors.setError(Errors::undefinedRule(symbol, _options->_converter));
        }

        // create empty rule
        out = Rule::empty(symbol);
    }
}

void
Registry::resetEvaluationContext()
{
    // todo: this shit
}