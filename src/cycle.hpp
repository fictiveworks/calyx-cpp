#pragma once

#include <memory>

#include "options.hpp"

namespace calyx
{
    class Cycle
    {
    public:

        static Cycle create(std::shared_ptr<Options> options, size_t count, ErrorHolder& errors);

        Cycle(const Cycle& old);

        Cycle& operator=(const Cycle& other);

        ~Cycle();
        
        void shuffle();

        int poll();

    protected:
        Cycle(std::shared_ptr<Options> options, size_t count);

    private:

        std::shared_ptr<Options> _options;
        size_t _index;
        
        size_t _count;
        int* _sequence;

        void populateSequence();

    };
}