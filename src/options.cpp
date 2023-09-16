#include "options.hpp"
#include <stdlib.h>
#include <exception>

using namespace calyx;

const bool Options::DEFAULT_STRICT = false;

Options::Options(bool strict, std::unique_ptr<StringConverter<String_t>> converter)
    : _strict(strict),
    _converter(std::move(converter))
{
}

Options::Options(unsigned int seed, bool strict, std::unique_ptr<StringConverter<String_t>> converter)
    : _strict(strict),
    _rng(seed),
    _converter(std::move(converter))
{

}

Options::Options(std::mt19937 rng, bool strict, std::unique_ptr<StringConverter<String_t>> converter)
    : _strict(strict),
    _rng(rng),
    _converter(std::move(converter))
{
}

int
Options::randInt()
{
    auto distribution = std::uniform_int_distribution(
        std::numeric_limits<int>::min(), 
        std::numeric_limits<int>::max()
    );

    return distribution(_rng);
}

double
Options::randDouble()
{
    std::uniform_real_distribution<double> distribution(0, 1);

    return distribution(_rng);
}

int
Options::randInt(int max, ErrorHolder& errorHolder)
{
    if (max <= 0)
    {
        String_t msg = _converter->fromString("Max bound must be positive");
        errorHolder.setError(msg);
        return 0;
    }

    auto distribution = std::uniform_int_distribution(0, max);
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
    auto distribution = std::uniform_int_distribution(min, max);
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