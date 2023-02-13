#pragma once

#include <string>
#include <vector>
#include <map>

#include "calyx.h"
#include "options.hpp"
#include "production.hpp"
#include "rule.hpp"

namespace calyx
{
    class Registry
    {
    public:
        Registry();

        Registry(Options options);

        ~Registry();

        Registry &operator=(const Registry &other);

        void defineRule(String_t term, std::vector<String_t> production);

    private:
        std::map<String_t, Rule> _rules;
        Options *_options;
    };
}
