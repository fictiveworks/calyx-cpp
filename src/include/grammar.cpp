#include "grammar.h"

using namespace calyx;

Grammar::Grammar() noexcept :
    Grammar(Options())
{
}

Grammar::Grammar(Options opts) noexcept :
    _registry(std::move(opts))
{
}

Grammar::Grammar(bool strict) noexcept :
    _registry(strict)
{
}

Grammar::Grammar(std::mt19937 rng, bool strict) noexcept :
    _registry(Options(rng, strict))
{
}

Grammar::Grammar(int seed, bool strict) noexcept :
    _registry(Options(seed, strict))
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
Grammar::rule(String_t term, String_t production, ErrorHolder& errors) noexcept
{
    _registry.defineRule(std::move(term), { std::move(production) }, errors);
}

void
Grammar::rule(String_t term, const std::vector<String_t>& production, ErrorHolder& errors) noexcept
{
    _registry.defineRule(std::move(term), production, errors);
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