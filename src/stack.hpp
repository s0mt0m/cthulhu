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
            data.pop();
            return value;
        }

        push( word value )
        {
            data.push_back( value );
        }
    };

} // namespace cthu

