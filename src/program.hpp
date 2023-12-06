#pragma once

#include "stack.hpp"

namespace cthu
{
    struct program
    {
        std::array< word, 8 > slots;
        std::vector< stack > stacks;

        template< typename value_t >
        value_t pop( word pos )
        {
            return stacks[ slots[ pos ] ].pop();
        }

        template< typename value_t >
        void push( word pos, value_t value )
        {
            stacks[ slots[ pos ] ].push( value );
        }

        word add_stack( stack s )
        {
            stacks.push_back( std::move( s ) );
            return stacks.size() - 1;
        }

        void set_inits( const std::vector< word > &ids )
        {
            ASSERT( ids.size() == 8 );
            for ( word i = 0; i < 8; ++i )
                slots[ i ] = ids[ i ];
        }

        bool halted() const
        {
            return stacks[ slots[ 0 ] ].empty();
        }

        word next_instruction()
        {
            return pop< word >( 0 );
        }
    };

} // namespace cthu

