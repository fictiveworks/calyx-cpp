#include "registry.h"

using namespace calyx;


Registry::Registry(): Registry(std::make_shared<Options>())
{
}

Registry::Registry(Options options): Registry(std::make_shared<Options>(std::move(options)))
{
}

Registry::Registry(std::shared_ptr<Options> options):
    _rules(std::map<String_t, std::shared_ptr<Rule>>()),
    _options(std::move(options))
{
    addFilter(_options->fromString("uppercase"), filters::uppercase);
}

Registry::Registry(Registry&& old) noexcept:
    _rules(std::move(old._rules)),
    _context(std::move(old._context)),
    _memos(std::move(old._memos)),
    _cycles(std::move(old._cycles)),
    _filters(std::move(old._filters)),
    _options(std::move(old._options))
{
}

Registry& Registry::operator=(Registry&& other) noexcept
{
    if (this != &other)
    {
        _rules = std::move(other._rules);
        _context = std::move(other._context);
        _memos = std::move(other._memos);
        _cycles = std::move(other._cycles);
        _options = std::move(other._options);
    }
    return *this;
}

Options&
Registry::getOptions() const
{
    return *_options;
}

void
Registry::defineRule(String_t term, const std::vector<String_t>& production, ErrorHolder& errors)
{
    std::optional<Rule> rule = Rule::build(term, production, *this, errors);

    if (!rule || errors.hasError())
    {
        return;
    }

    _rules.emplace(std::move(term), std::make_shared<Rule>(*rule));
}

void Registry::addFilter(String_t name, Filter_t filter)
{
    _filters.emplace(std::move(name), filter);
}

std::optional<const Filter_t>
Registry::getFilter(const String_t& name) const
{
    if (_filters.contains(name))
    {
        return _filters.at(name);
    }
    return {};
}

std::optional<Expansion>
Registry::evaluate(const String_t& startSymbol, ErrorHolder& errors)
{
    this->resetEvaluationContext();

    const std::shared_ptr<Rule> rule = this->expand(startSymbol, errors);

    if (rule == nullptr || errors.hasError())
    {
        return {};
    }

    std::optional<Expansion> tail = rule->evaluate(*this, getOptions(), errors);
    if (!tail || errors.hasError())
    {
        return {};
    }

    Expansion root(Exp::RESULT, *tail);

    return root;
}

std::optional<Expansion>
Registry::evaluate(const String_t& startSymbol, std::map<String_t, std::vector<String_t>> context, ErrorHolder& errors)
{
    this->resetEvaluationContext();

    // use a temp builder since we dont want to affect class context if an error occurs
    std::map<String_t, std::shared_ptr<Rule>> contextBuilder;
    for (const auto& rule : context)
    {
        std::optional<Rule> contextRule = Rule::build(rule.first, rule.second, *this, errors);
        if (!contextRule || errors.hasError())
        {
            return {};
        }

        contextBuilder[rule.first] = std::make_shared<Rule>(*contextRule);
    }
    // we can move since the class member has been reset and is thus empty right now
    _context = std::move(contextBuilder);

    const std::shared_ptr<Rule> rule = this->expand(startSymbol, errors);

    if (rule == nullptr || errors.hasError())
    {
        return {};
    }

    std::optional<Expansion> tail = rule->evaluate(*this, getOptions(), errors);

    if (!tail)
    {
        return {};
    }

    return Expansion(Exp::RESULT, *tail);
}

std::shared_ptr<Expansion>
Registry::memoizeExpansion(const String_t& symbol, ErrorHolder& errors)
{
    if (!_memos.contains(symbol))
    {
        auto rule = this->expand(symbol, errors);
        if (!rule || errors.hasError())
        {
            return nullptr;
        }

        std::optional<Expansion> exp = rule->evaluate(*this, *_options, errors);

        if (!exp || errors.hasError())
        {
            return nullptr;
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
        std::shared_ptr<Rule> prod = this->expand(symbol, errors);

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


    return rule->evaluateAt(_cycles[symbol]->poll(), *this, *_options, errors);
}

std::shared_ptr<Rule>
Registry::expand(const String_t& symbol, ErrorHolder& errors) const
{
    if (_rules.contains(symbol))
    {
        return _rules.at(symbol);
    }

    if (_context.contains(symbol))
    {
        return _context.at(symbol);
    }

    if (_options->isStrict())
    {
        // strict - do not allow empty rules
        errors.setError(Errors::undefinedRule(symbol, *_options));
        return nullptr;
    }

    // create empty rule
    return std::make_shared<Rule>(Rule::empty(symbol));
}

void
Registry::resetEvaluationContext()
{
    _cycles.clear();
    _context.clear();
    _memos.clear();
}