#include "options.hpp"

using namespace calyx;

const bool
Options::DEFAULT_STRICT = false;

Options::Options(bool strict = DEFAULT_STRICT, const StringConverter<String_t> &converter = StringConverters::CALYX_STRING_CONVERTER):
    _strict(strict),
    _converter(converter),
    _rng([]() { return 0; }) // TODO: Find actual implementation for rng
{}

Options::Options(int seed, bool strict = DEFAULT_STRICT, const StringConverter<String_t> &converter = StringConverters::CALYX_STRING_CONVERTER):
    _strict(strict),
    _converter(converter),
    _rng([seed]() { return seed; }) // TODO: Find actual implementation for rng
{}

Options::Options(std::function<int()> rng, bool strict = DEFAULT_STRICT, const StringConverter<String_t> &converter = StringConverters::CALYX_STRING_CONVERTER):
    _strict(strict),
    _converter(converter),
    _rng(rng)
{}

Options::~Options()
{}
