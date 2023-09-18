#include "options.h"
#include <stdlib.h>
#include <exception>

using namespace calyx;

const bool Options::DEFAULT_STRICT = false;

Options::Options(bool strict, std::unique_ptr<StringConverter> converter):
    _strict(strict),
    _rng(),
    _converter(std::move(converter))
{
}

Options::Options(int seed, bool strict, std::unique_ptr<StringConverter> converter):
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


int
Options::randInt()
{
    const std::uniform_int_distribution distribution(
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max()
    );

    return distribution(_rng);
}

double
Options::randDouble()
{
    const std::uniform_real_distribution distribution(0.0, 1.0);

    return distribution(_rng);
}

int
Options::randInt(int max, ErrorHolder& errorHolder)
{
    if (max <= 0)
    {
        const String_t msg = _converter->fromString("Max bound must be positive");
        errorHolder.setError(msg);
        return 0;
    }

    const std::uniform_int_distribution distribution(0, max);
    return distribution(_rng);
}

int
Options::randInt(int min, int max, ErrorHolder& errorHolder)
{
    if (max <= 0)
    {
        String_t msg = _converter->fromString("Max bound must be positive");
        errorHolder.setError(msg);
        return 0;
    }

    if (max <= min)
    {
        String_t msg = _converter->fromString("Max bound must be greater than min!");
        errorHolder.setError(msg);
        return 0;
    }
    const std::uniform_int_distribution distribution(min, max);
    return distribution(_rng);
}

int
Options::randInt(int max)
{
    ErrorHolder errs;

    return randInt(max, errs);
}

int
Options::randInt(int min, int max)
{
    ErrorHolder errs;

    return randInt(min, max, errs);
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