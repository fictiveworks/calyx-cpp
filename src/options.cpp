#include "options.h"

using namespace calyx;

const bool Options::DEFAULT_STRICT = false;

Options::Options(bool strict, std::unique_ptr<StringConverter> converter):
    _strict(strict),
    _rng(),
    _converter(std::move(converter))
{
}

Options::Options(unsigned long seed, bool strict, std::unique_ptr<StringConverter> converter):
    _strict(strict),
    _rng(seed),
    _converter(std::move(converter))
{
}

Options::Options(std::mt19937 rng, bool strict, std::unique_ptr<StringConverter> converter):
    _strict(strict),
    _rng(rng),
    _converter(std::move(converter))
{
}

Options::Options(Options&& other) noexcept:
    _strict(other._strict),
    _rng(other._rng),
    _converter(std::move(other._converter))
{
}

Options& Options::operator=(Options&& old) noexcept
{
    if (this != &old)
    {
        _strict = old._strict;
        _rng = old._rng;
        _converter = std::move(old._converter);
    }

    return *this;
}

bool
Options::isStrict() const
{
    return _strict;
}

std::string
Options::toString(const String_t& stringLike) const
{
    return _converter->toString(stringLike);
}

String_t
Options::fromString(const std::string& stdString) const
{
    return _converter->fromString(stdString);
}