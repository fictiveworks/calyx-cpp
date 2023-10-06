#include "cycle.h"

#include <algorithm>

using namespace calyx;

Cycle::Cycle(std::shared_ptr<Options> options, size_t count)
    : _options(options),
    _count(count), // assume count greater than 1
    _sequence(new int[count])
{
    _index = _count - 1;  // defer shuffling until the first poll()
}

Cycle::Cycle(const Cycle& old)
    : _options(old._options),
    _index(old._index),
    _count(old._count),
    _sequence(new int[_count])
{

}

Cycle::~Cycle()
{
    delete[] _sequence;
}

std::optional<Cycle>
Cycle::create(std::shared_ptr<Options> options, size_t count, ErrorHolder& errors)
{
    if (count < 1)
    {
        errors.setError(options->fromString("'count' must be greater than zero!"));
        return {};
    }

    return Cycle(options, count);
}

Cycle&
Cycle::operator=(const Cycle& other)
{
    if (this == &other)
    {
        return *this;
    }
    
    delete[] _sequence;

    _options = other._options;
    _index = other._index;

    _count = other._count;
    _sequence = new int[other._count];
    
    std::copy(other._sequence, other._sequence + _count, _sequence);

    return *this;
}

void
Cycle::shuffle()
{
    this->populateSequence();
    size_t current = _count;

    while (current > 1)
    {
        std::size_t target = _options->randomInteger(current);
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