#pragma once

#include "stack.hpp"

#include <map>

namespace cthu
{
    struct dictionary
    {
        std::map< word, stack > data;

        word pop( word idx )
        {
            return data.at( idx ).pop();
        }

        void push( word idx, word value )
        {
            data.at( idx ).push( value );
        }

        bool empty( word idx ) const
        {
            return data.at( idx ).empty();
        }
    };

} // namespace cthu

