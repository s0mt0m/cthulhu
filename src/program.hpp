#pragma once

#include "stack.hpp"

namespace cthu
{
    struct program
    {
        std::array< stack > slots;

        template< typename value_t >
        value_t pop( word pos )
        {
            return slots[ pos ].pop();
        }

        template< typename value_t >
        void push( word pos, value_t value )
        {
            slots[ pos ].push( value );
        }
    };

} // namespace cthu

