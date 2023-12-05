#pragma once

#include "types.hpp"

#include <vector>

namespace cthu
{
    struct stack
    {
        std::vector< word > data;

        word pop()
        {
            const word value = data.back();
            data.pop_back();
            return value;
        }

        void push( word value )
        {
            data.push_back( value );
        }

        bool empty() const
        {
            return data.empty();
        }
    };

} // namespace cthu

