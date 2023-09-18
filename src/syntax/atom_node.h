#pragma once

#include <optional>
#include "../registry.h"
#include "../production.h"
#include "../expansion.h"
#include "../include/string_converter.h"
#include "../options.h"

namespace calyx
{

    class AtomNode : public Production
    {
    public:

        AtomNode(String_t atom);

        std::optional<Expansion> evaluate(
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const override;

    private:

        String_t _atom;

    };
}