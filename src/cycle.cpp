#include "cycle.h"

#include <algorithm>
#include <iostream>

using namespace calyx;

Cycle::Cycle() noexcept:
    _count(1),
    _index(0)
{
}

std::optional<Cycle> Cycle::create(std::size_t count, StringConverter<String_t>& converter, ErrorHolder& errs) noexcept
{
    if (count < 1)
    {
        errs.setError(converter.fromString("Cycle count must be greater than zero!"));
        return {};
    }
    
    return Cycle(count);
}

Cycle::Cycle(std::size_t count) noexcept: 
    _count(count)
{
    _index = _count - 1;
}

Cycle::Cycle(Cycle&& other) noexcept: 
    _count(other._count),
    _index(other._index),
    _sequence(std::move(other._sequence))
{
    other._sequence.clear();
    other._count = 0;
    other._index = 0;
}

Cycle& Cycle::operator=(const Cycle& other) noexcept
{
    if (this != &other)
    {
        _count = other._count;
        _index = other._index;
        _sequence = other._sequence;
    }
    
    return *this;
}

Cycle& Cycle::operator=(Cycle&& other) noexcept
{
    if (this != &other)
    {
        _count = other._count;
        _index = other._index;
        _sequence = std::move(other._sequence);
    }
    
    return *this;
}

void
Cycle::shuffle(Options& options) noexcept
{
    std::cout << "RNG State before shuffling: " 
          << options.randomInteger(0, 10) << ", " 
          << options.randomInteger(0, 10) << std::endl;
    this->populateSequence();
    options.shuffle(_sequence);
}

std::size_t
Cycle::poll(Options& options) noexcept
{
    _index++;
    if (_index >= _count)
    {
        this->shuffle(options);
        _index = 0;
    }
    return _sequence[_index];
}

void
Cycle::populateSequence() noexcept
{
    _sequence.clear();
    _sequence.reserve(_count);
    for (size_t i = 0; i < _count; i++)
    {
        _sequence.push_back(i);
    }
}

const std::vector<std::size_t>&
Cycle::getSequence() const noexcept
{
    return _sequence;
}
