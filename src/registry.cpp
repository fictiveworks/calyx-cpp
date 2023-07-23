#include "registry.hpp"

using namespace calyx;

Registry::Registry(): Registry(std::make_shared<Options>(Options()))
{
}

Registry::Registry(std::shared_ptr<Options> options)
    : _options(options),
    _rules(std::map<String_t, Rule>())
{
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
    _options = other._options;

    return *this;
}


void
Registry::defineRule(String_t term, std::vector<String_t> production, ErrorHolder& errors)
{
    std::optional<Rule> rule = Rule::build(term, production, *this, errors);

    if (!rule)
    {
        return;
    }

    _rules[term] = *rule;
}

std::optional<Expansion>
Registry::evaluate(const String_t& startSymbol, ErrorHolder& errors)
{
    this->resetEvaluationContext();

    auto rule = this->expand(startSymbol, errors);

    if (!rule)
    {
        return {};
    }

    std::optional<Expansion> tail = rule->evaluate(getOptions(), errors);

    if (!tail)
    {
        return {};
    }

    Expansion root = Expansion(RESULT, *tail);

    return root;
}

std::optional<Expansion>
Registry::evaluate(const String_t& startSymbol, std::map<String_t, std::vector<String_t>> context, ErrorHolder& errors)
{
    this->resetEvaluationContext();

    // TODO: build context from rules
    // for (const auto& rule : context)
    // {
    //     // C# version has a commented-out exception for duplicate rules here, idk
    //     _context[rule.first] = Rule::build(rule.first, rule.second, *this);
    // }

    std::optional<Rule> rule = this->expand(startSymbol, errors);

    if (!rule)
    {
        return {};
    }

    std::optional<Expansion> tail = rule->evaluate(getOptions(), errors);

    if (!tail)
    {
        return {};
    }

    return Expansion(RESULT, *tail);
}

std::optional<std::shared_ptr<Expansion>>
Registry::memoizeExpansion(const String_t& symbol, ErrorHolder& errors)
{
    if (!_memos.contains(symbol))
    {
        auto rule = this->expand(symbol, errors);
        if (!rule || errors.hasError())
        {
            return {};
        }

        std::optional<Expansion> exp = rule->evaluate(*_options, errors);

        if (!exp || errors.hasError())
        {
            return {};
        }

        _memos[symbol] = std::make_shared<Expansion>(*exp);
    }

    return _memos[symbol];
}

std::optional<Expansion>
Registry::uniqueExpansion(const String_t& symbol, ErrorHolder& errors)
{
    //
    if (!_cycles.contains(symbol))
    {
        std::optional<Rule> prod = this->expand(symbol, errors);

        if (!prod || errors.hasError()) 
        {
            return {};
        }

        size_t cycleLength = prod->length();

        std::optional<Cycle> cycle = Cycle::create(_options, cycleLength, errors);

        if (!cycle || errors.hasError()) 
        {
            return {};
        }
        _cycles.emplace(symbol, std::make_unique<Cycle>(cycle.value()));
    }

    auto rule = this->expand(symbol, errors);

    if (!rule || errors.hasError())
    {
        return {};
    }


    return rule->evaluateAt(_cycles[symbol]->poll(), *_options, errors);
}

std::optional<Rule>
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
            errors.setError(Errors::undefinedRule(symbol, _options->_converter));

            return {};
        }

        // create empty rule
        return Rule::empty(symbol);
    }
}

void
Registry::resetEvaluationContext()
{
    _cycles.clear();
    _context.clear();
    _memos.clear();
}