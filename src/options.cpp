#include "options.hpp"
#include <stdlib.h>
#include <exception>

using namespace calyx;

const bool Options::DEFAULT_STRICT = false;

Options::Options(bool strict, std::unique_ptr<StringConverter<String_t>> converter)
    : _strict(strict),
    _converter(std::move(converter)),
    _rng(std::rand) // TODO: Find actual implementation for rng
{
}

Options::Options(unsigned int seed, bool strict, std::unique_ptr<StringConverter<String_t>> converter)
    : _strict(strict),
    _converter(std::move(converter)),
    _rng(std::rand) // TODO: Find actual implementation for rng
{
    std::srand(seed); // TODO: Find actual implementation for seeded rng
}

Options::Options(std::function<int()> rng, bool strict, std::unique_ptr<StringConverter<String_t>> converter)
    : _strict(strict),
    _converter(std::move(converter)),
    _rng(rng)
{
}

Options::~Options()
{
}

int 
Options::randInt()
{
    return _rng();
}

double
Options::randDouble()
{
    int randint = randInt(1000);

    return randint / 1000.0;
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

    int random = randInt();
    return ((random % max) + max) % max; // TODO: modulo isnt the best way to bound random numbers
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

    return randInt(max - min, errorHolder) + min;
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

const String_t 
Options::toString(const String_t& stringLike) const
{
    return _converter->toString(stringLike);
}

const String_t 
Options::fromString(const std::string& stdString) const
{
    return _converter->fromString(stdString);
}