#include "calyx.h"

using namespace calyx;

Grammar::Grammar() noexcept :
    Grammar(Options())
{
}

Grammar::Grammar(Options opts) noexcept :
    _registry(std::move(opts))
{
    const BaseFilters baseFilters = BaseFilters();
    this->filters(baseFilters);
}

Grammar::Grammar(bool strict) noexcept :
    _registry(strict)
{
    const BaseFilters baseFilters = BaseFilters();
    this->filters(baseFilters);
}

Grammar::Grammar(std::mt19937 rng, bool strict) noexcept :
    _registry(Options(rng, strict))
{
    const BaseFilters baseFilters = BaseFilters();
    this->filters(baseFilters);
}

Grammar::Grammar(unsigned long seed, bool strict) noexcept :
    _registry(Options(seed, strict))
{
    const BaseFilters baseFilters = BaseFilters();
    this->filters(baseFilters);
}

Grammar::Grammar(const std::function<void(Grammar&)>& initializeCallback, bool strict) noexcept :
    Grammar(strict)
{
    const BaseFilters baseFilters = BaseFilters();
    this->filters(baseFilters);
    initializeCallback(*this);
}

Grammar::Grammar(const std::function<void(Grammar&)>& initializeCallback, std::mt19937 rng, bool strict) noexcept :
    Grammar(rng, strict)
{
    const BaseFilters baseFilters = BaseFilters();
    this->filters(baseFilters);
    initializeCallback(*this);
}

Grammar::Grammar(const std::function<void(Grammar&)>& initializeCallback, unsigned long seed, bool strict) noexcept :
    Grammar(seed, strict)
{
    const BaseFilters baseFilters = BaseFilters();
    this->filters(baseFilters);
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
    const Options& ops = _registry.getOptions();
    std::optional<Expansion> exp = _registry.evaluate(ops.fromString("start"), errors);
    if (!exp || errors.hasError())
    {
        return {};
    }
    return Result(*exp);
}

void
Grammar::filters(const FiltersProvider& provider) noexcept
{
    std::map<String_t, Filter_t> filters = provider.getFilters(_registry.getOptions());

    for (const auto& pair : filters)
    {
        _registry.addFilter(pair.first, pair.second);
    }
}

Options& Grammar::getOptions() const noexcept
{
    return _registry.getOptions();
}


