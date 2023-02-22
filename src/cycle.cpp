#include "cycle.hpp"

using namespace calyx;

Cycle::Cycle(Options options, size_t count)
    : _options(options),
    _count(count) // assume count greater than 1
{
    _sequence = new int[count];
    _index = _count - 1;  // defer shuffling until the first poll()
}

Cycle::~Cycle()
{
    delete[] _sequence;
}

Cycle
Cycle::create(Options options, size_t count, ErrorHolder& errors)
{
    if (count < 1)
    {
        errors.setError(options._converter.fromString("'count' must be greater than zero!"));
        return Cycle(options, -1);
    }

    return Cycle(options, count);
}

void
Cycle::shuffle()
{
    this->populateSequence();
    size_t current = _count;

    while (current > 1)
    {
        size_t target = _options.randInt(current);
        current--;
        std::swap(_sequence[current], _sequence[target]);
    }
}

int
Cycle::poll()
{
    _index++;
    if (_index >= _count)
    {
        this->shuffle();
        _index = 0;
    }
    return _sequence[_index];
}

void
Cycle::populateSequence()
{
    for (size_t i = 0; i < _count; i++)
    {
        _sequence[i] = i;
    }
}