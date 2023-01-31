#include "options.hpp"
#include <stdlib.h>

using namespace calyx;

const bool
Options::DEFAULT_STRICT = false;

Options::Options(bool strict = DEFAULT_STRICT, const StringConverter<String_t> &converter = StringConverters::CALYX_STRING_CONVERTER):
    _strict(strict),
    _converter(converter),
    _rng(std::rand) // TODO: Find actual implementation for rng
{}

Options::Options(unsigned int seed, bool strict = DEFAULT_STRICT, const StringConverter<String_t> &converter = StringConverters::CALYX_STRING_CONVERTER):
    _strict(strict),
    _converter(converter),
    _rng(std::rand) // TODO: Find actual implementation for rng
{
    std::srand(seed); // TODO: Find actual implementation for seeded rng
}

Options::Options(std::function<int()> rng, bool strict = DEFAULT_STRICT, const StringConverter<String_t> &converter = StringConverters::CALYX_STRING_CONVERTER):
    _strict(strict),
    _converter(converter),
    _rng(rng)
{}

Options::Options(const Options& old):
    _strict(old._strict),
    _converter(old._converter),
    _rng(old._rng)
{}

Options::~Options()
{}

int
Options::randInt()
{
    return _rng();
}

int
Options::randInt(int max)
{
    if (max <= 0) {
        return 0; // TODO: this should generate an error
    }

    return randInt() % max;
}

int
Options::randInt(int min, int max)
{
    if (max <= 0) {
        return 0; // TODO: this should generate an error
    }

    if (max <= min) {
        return 0; // TODO: this should generate an error
    }

    return randInt(max - min) + min;
}
