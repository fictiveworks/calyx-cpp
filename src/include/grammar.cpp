#include "grammar.h"

using namespace calyx;

Grammar::Grammar() noexcept : Grammar(Options())
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

void Grammar::start(String_t production) noexcept
{
}

void Grammar::start(std::vector<String_t> production) noexcept
{
}

void Grammar::rule(String_t term, String_t production) noexcept
{
}

void Grammar::rule(std::vector<String_t> term, std::vector<String_t> production) noexcept
{
}

Result Grammar::generate() const noexcept
{
}