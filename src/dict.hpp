#pragma once

#include "stack.hpp"

#include <map>

namespace cthu
{
    struct dictionary
    {
        std::vector< stack > &stacks;
        std::map< word, word > data;

        dictionary( std::vector< stack > &stacks )
            : stacks( stacks )
        {}

        dictionary( std::vector< stack > &stacks, std::map< word, word > data )
            : stacks( stacks ), data( data )
        {}

        word pop( word idx )
        {
            return stacks[ data.at( idx ) ].pop();
        }

        void push( word idx, word value )
        {
            stacks[ data.at( idx ) ].push( value );
        }

        bool empty( word idx ) const
        {
            return stacks[ data.at( idx ) ].empty();
        }
    };

} // namespace cthu

