#include "grammar.h"

using namespace calyx;

Grammar::Grammar() noexcept :
    Grammar(Options())
{
}

Grammar::Grammar(Options opts) noexcept :
    _registry(std::move(opts))
{
    const std::map<String_t, filters::Filter_t> filters = filters::createBuiltinFilters(_registry.getOptions());
    this->filters(filters);
}

Grammar::Grammar(bool strict) noexcept :
    Grammar(Options(strict))
{
}

Grammar::Grammar(std::mt19937 rng, bool strict) noexcept :
    Grammar(Options(rng, strict))
{
}

Grammar::Grammar(unsigned long seed, bool strict) noexcept :
    Grammar(Options(seed, strict))
{
}

Grammar::Grammar(const std::function<void(Grammar&)>& initializeCallback, bool strict) noexcept :
    Grammar(strict)
{
    initializeCallback(*this);
}

Grammar::Grammar(const std::function<void(Grammar&)>& initializeCallback, std::mt19937 rng, bool strict) noexcept :
    Grammar(rng, strict)
{
    initializeCallback(*this);
}

Grammar::Grammar(const std::function<void(Grammar&)>& initializeCallback, unsigned long seed, bool strict) noexcept :
    Grammar(seed, strict)
{
    initializeCallback(*this);
}

Grammar::Grammar(Grammar&& old) noexcept :
    _registry(std::move(old._registry))
{
}

Grammar& Grammar::operator=(Grammar&& old) noexcept
{
    if (this != &old)
    {
        _registry = std::move(old._registry);
    }

    return *this;
}

void
Grammar::start(String_t production, ErrorHolder& errors) noexcept
{
    const Options& ops = _registry.getOptions();
    _registry.defineRule(ops.fromString("start"), { std::move(production) }, errors);
}

void
Grammar::start(const std::vector<String_t>& production, ErrorHolder& errors) noexcept
{
    const Options& ops = _registry.getOptions();
    _registry.defineRule(ops.fromString("start"), production, errors);
}

void
Grammar::start(const std::map<String_t, double>& productions, ErrorHolder& errors) noexcept
{
    const Options& ops = _registry.getOptions();
    _registry.defineRule(ops.fromString("start"), productions, errors);
}


void
Grammar::rule(String_t term, String_t production, ErrorHolder& errors) noexcept
{
    _registry.defineRule(std::move(term), { std::move(production) }, errors);
}

void
Grammar::rule(String_t term, const std::vector<String_t>& production, ErrorHolder& errors) noexcept
{
    _registry.defineRule(std::move(term), production, errors);
}

void
Grammar::rule(String_t term, const std::map<String_t, double>& productions, ErrorHolder& errors) noexcept
{
    _registry.defineRule(std::move(term), productions, errors);
}

std::optional<Result>
Grammar::generate(ErrorHolder& errors) noexcept
{
    return generate(getOptions().fromString("start"), errors);
}

std::optional<Result>
Grammar::generate(const String_t& start, ErrorHolder& errors) noexcept
{
    std::optional<Expansion> exp = _registry.evaluate(start, errors);
    if (!exp || errors.hasError())
    {
        return {};
    }
    return Result(*exp);
}

void
Grammar::filters(const std::map<String_t, filters::Filter_t>& filters) noexcept
{
    for (const auto& pair : filters)
    {
        _registry.addFilter(pair.first, pair.second);
    }
}

void Grammar::filter(String_t filterName, filters::Filter_t filter) noexcept
{
    _registry.addFilter(std::move(filterName), std::move(filter));
}


Options& Grammar::getOptions() const noexcept
{
    return _registry.getOptions();
}


