#pragma once
#include "../production.h"
#include "../include/string_converter.h"

namespace calyx
{
    class UniqueNode: public Production
    {
    public:
        UniqueNode(String_t symbol);

        ~UniqueNode() override = default;

        virtual std::optional<Expansion> evaluate(
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const override;
    
    private:
        String_t _symbol;
    };
}
