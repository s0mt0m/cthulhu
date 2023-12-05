#pragma once

#include "types.hpp"

#include <cassert>

namespace cthu
{
    template< word count >
    constexpr word ones = ( 1u << count ) - 1;

    constexpr word bvec_op( word opcode )
    {
        return ( opcode >> 12 ) & ones< 8 >;
    }

    template< word idx >
    constexpr word bvec_arg( word opcode )
    {
        if constexpr ( idx == 2 )
            return opcode & ones< 4 >;
        else
            return ( opcode >> ( 4 * idx ) ) & ones< 4 >;
    }

    consteval word make_opcode( word stid, word opid )
    {
        if ( stid == 0 )
            return opid;
        else
            return ( stid << 20 ) | ( opid << 12 );
    }

    constexpr word bvec_args( word a, word b, word c )
    {
        assert( a < 8 );
        assert( b < 8 );
        assert( c < 8 );

        return ( a << 8 ) | ( b << 4 ) | c;
    }

    template< typename T, word op >
    constexpr word opcode = make_opcode( type_id< T >, op );

    #define OPDEF( NAME, ID )                       \
        template< typename T = void >               \
        constexpr word NAME = opcode< T, ID >       \

    OPDEF( bvec_add,  0x20 );
    OPDEF( bvec_sub,  0x21 );
    OPDEF( bvec_mul,  0x22 );
    OPDEF( bvec_sdiv, 0x23 );
    OPDEF( bvec_udiv, 0x24 );
    OPDEF( bvec_srem, 0x25 );
    OPDEF( bvec_urem, 0x26 );

    #undef OPDEF

    #define OPCODE( NAME, TYPE, IMPL )                  \
        constexpr word NAME( word a, word b, word c )   \
        {                                               \
            return IMPL< TYPE > | bvec_args( a, b, c ); \
        }

    OPCODE( byte_add,  byte, bvec_add  )
    OPCODE( byte_sub,  byte, bvec_sub  )
    OPCODE( byte_mul,  byte, bvec_mul  )
    OPCODE( byte_sdiv, byte, bvec_sdiv )
    OPCODE( byte_udiv, byte, bvec_udiv )
    OPCODE( byte_srem, byte, bvec_srem )
    OPCODE( byte_urem, byte, bvec_urem )

    OPCODE( half_add,  half, bvec_add  )
    OPCODE( half_sub,  half, bvec_sub  )
    OPCODE( half_mul,  half, bvec_mul  )
    OPCODE( half_sdiv, half, bvec_sdiv )
    OPCODE( half_udiv, half, bvec_udiv )
    OPCODE( half_srem, half, bvec_srem )
    OPCODE( half_urem, half, bvec_urem )

    OPCODE( word_add,  word, bvec_add  )
    OPCODE( word_sub,  word, bvec_sub  )
    OPCODE( word_mul,  word, bvec_mul  )
    OPCODE( word_sdiv, word, bvec_sdiv )
    OPCODE( word_udiv, word, bvec_udiv )
    OPCODE( word_srem, word, bvec_srem )
    OPCODE( word_urem, word, bvec_urem )

    #undef OPCODE

} // namespace cthu

