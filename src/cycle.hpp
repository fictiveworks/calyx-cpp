#pragma once

#include <array>

#include "options.hpp"

namespace calyx
{
    class Cycle
    {
    public:

        static Cycle create(Options options, size_t count, ErrorHolder& errors);

        ~Cycle();
        
        void shuffle();

        int poll();

    protected:
        Cycle(Options options, size_t count);

    private:

        Options _options;
        size_t _index;
        
        size_t _count;
        int* _sequence;

        void populateSequence();

    };
}