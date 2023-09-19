#pragma once
#include "../production.h"

namespace calyx
{
    class MemoNode : public Production
    {
    public:
        MemoNode(String_t symbol);

        ~MemoNode() override = default;

        std::optional<Expansion> evaluate(
            Registry& registry,
            Options& options,
            ErrorHolder& errors
        ) const override;
        
    private:
        String_t _symbol;
    };
}