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
    this->resetEvaluationContext();

    Rule rule = this->expand(startSymbol, errors);

    if (errors.hasError()) {
        return Expansion(ERROR, _options->_converter.fromString(""));
    }

    Expansion root = Expansion(RESULT, std::make_shared<Expansion>(rule.evaluate(getOptions())));

    return root;
}

Expansion
Registry::evaluate(const String_t& startSymbol, std::map<String_t, std::vector<String_t>> context, ErrorHolder& errors)
{
    this->resetEvaluationContext();

    for (const auto& rule : context)
    {
        // C# version has a commented-out exception for duplicate rules here, idk
        _context[rule.first] = Rule::build(rule.first, rule.second, *this);
    }

    Rule rule = this->expand(startSymbol, errors);
    std::shared_ptr<Expansion> tail = std::make_shared<Expansion>(rule.evaluate(this->getOptions()));

    return Expansion(RESULT, tail);
}

Expansion 
Registry::memoizeExpansion(const String_t& symbol, ErrorHolder& errors)
{
    if (!_memos.contains(symbol))
    {
        _memos[symbol] = this->expand(symbol, errors).evaluate(this->getOptions());
    }

    return _memos[symbol];
}


Rule
Registry::expand(const String_t& symbol, ErrorHolder& errors) const
{
    if (_rules.contains(symbol))
    {
        return _rules.at(symbol);
    }
    else if (_context.contains(symbol))
    {
        return _context.at(symbol);
    }
    else
    {
        if (_options->_strict) {
            // strict - do not allow empty rules
            // no early return since the empty rule will be returned anyway
            errors.setError(Errors::undefinedRule(symbol, _options->_converter));
        }

        // create empty rule
        return Rule::empty(symbol);
    }
}

void
Registry::resetEvaluationContext()
{
    _rules.clear();
    _context.clear();
    _memos.clear();
}