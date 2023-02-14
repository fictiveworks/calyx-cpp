#include "options.hpp"
#include <stdlib.h>
#include <exception>

using namespace calyx;

const bool Options::DEFAULT_STRICT = false;

Options::Options(bool strict = DEFAULT_STRICT, const StringConverter<String_t>& converter = StringConverters::DEFAULT_STRING_CONVERTER)
    : _strict(strict),
    _converter(converter),
    _rng(std::rand) // TODO: Find actual implementation for rng
{
}

Options::Options(unsigned int seed, bool strict = DEFAULT_STRICT, const StringConverter<String_t>& converter = StringConverters::DEFAULT_STRING_CONVERTER)
    : _strict(strict),
    _converter(converter),
    _rng(std::rand) // TODO: Find actual implementation for rng
{
    std::srand(seed); // TODO: Find actual implementation for seeded rng
}

Options::Options(std::function<int()> rng, bool strict = DEFAULT_STRICT, const StringConverter<String_t>& converter = StringConverters::DEFAULT_STRING_CONVERTER)
    : _strict(strict),
    _converter(converter),
    _rng(rng)
{
}

Options::Options(const Options& old)
    : _strict(old._strict),
    _converter(old._converter),
    _rng(old._rng)
{
}

Options::~Options()
{
}

int Options::randInt()
{
    return _rng();
}

int Options::randInt(int max, ErrorHolder& errorHolder)
{
    if (max <= 0)
    {
        String_t msg = _converter.fromString("Max bound must be positive");
        errorHolder.setError(msg);
        return 0;
    }

    int random = randInt();
    return ((random % max) + max) % max; // TODO: modulo isnt the best way to bound random numbers
}

int Options::randInt(int min, int max, ErrorHolder& errorHolder)
{
    if (max <= 0)
    {
        String_t msg = _converter.fromString("Max bound must be positive");
        errorHolder.setError(msg);
        return 0;
    }

    if (max <= min)
    {
        String_t msg = _converter.fromString("Max bound must be greater than min!");
        errorHolder.setError(msg);
        return 0;
    }

    return randInt(max - min, errorHolder) + min;
}

int Options::randInt(int max)
{
    IGNORE_ERRORS(return randInt, max);
}

int Options::randInt(int min, int max)
{
    IGNORE_ERRORS(return randInt, min, max);
}