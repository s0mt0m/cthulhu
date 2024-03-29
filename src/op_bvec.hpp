#pragma once

#include "types.hpp"
#include "utils.hpp"

#include <cassert>

namespace cthu
{
    constexpr word bvec_op( word opcode )
    {
        return ( opcode >> 12 ) & utils::ones< 8 >;
    }

    template< word idx >
    constexpr word bvec_arg( word opcode )
    {
        if constexpr ( idx == 2 )
            return opcode & utils::ones< 4 >;
        else
            return ( opcode >> ( 4 * idx ) ) & utils::ones< 4 >;
    }

    template< word count = 3 >
    constexpr auto bvec_args( word opcode )
    {
        return [ = ]< word... idx >( std::integer_sequence< word, idx... > )
        {
            return std::make_tuple( bvec_arg< idx >( opcode )... );
        }( std::make_integer_sequence< word, count >{} );
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

    OPDEF( bvec_add,     0x10 );
    OPDEF( bvec_sub,     0x11 );
    OPDEF( bvec_mul,     0x12 );
    OPDEF( bvec_sdiv,    0x13 );
    OPDEF( bvec_udiv,    0x14 );
    OPDEF( bvec_srem,    0x15 );
    OPDEF( bvec_urem,    0x16 );

    OPDEF( bvec_and,     0x20 );
    OPDEF( bvec_or,      0x21 );
    OPDEF( bvec_xor,     0x22 );
    OPDEF( bvec_neg,     0x23 );
    OPDEF( bvec_shl,     0x24 );
    OPDEF( bvec_lshr,    0x25 );
    OPDEF( bvec_ashr,    0x26 );

    OPDEF( bvec_eq,      0x30 );
    OPDEF( bvec_neq,     0x31 );
    OPDEF( bvec_ule,     0x32 );
    OPDEF( bvec_ult,     0x33 );
    OPDEF( bvec_ugt,     0x34 );
    OPDEF( bvec_uge,     0x35 );
    OPDEF( bvec_sle,     0x36 );
    OPDEF( bvec_slt,     0x37 );
    OPDEF( bvec_sgt,     0x38 );
    OPDEF( bvec_sge,     0x39 );

    // unsigned extension
    OPDEF( bvec_zext_b,  0x50 ); // byte → *
    OPDEF( bvec_zext_h,  0x51 ); // half → *
    // signed extension
    OPDEF( bvec_sext_b,  0x52 ); // byte → *
    OPDEF( bvec_sext_h,  0x53 ); // half → *
    // truncation
    OPDEF( bvec_trunc_h, 0x54 ); // half → *
    OPDEF( bvec_trunc_w, 0x55 ); // word → *

    #undef OPDEF

    #define OPCODE( NAME, TYPE, IMPL, PARAMS, ARGS )    \
        constexpr word NAME PARAMS                      \
        {                                               \
            return IMPL< TYPE > | bvec_args ARGS;       \
        }

    #define OPCODE_2( NAME, TYPE, IMPL )    \
        OPCODE( NAME, TYPE, IMPL, ( word a, word b ), ( a, b, 0 ) )

    #define OPCODE_3( NAME, TYPE, IMPL )    \
        OPCODE( NAME, TYPE, IMPL, ( word a, word b, word c ), ( a, b, c ) )

    // byte (8 bit) operations

    OPCODE_3( byte_add,     byte, bvec_add     )
    OPCODE_3( byte_sub,     byte, bvec_sub     )
    OPCODE_3( byte_mul,     byte, bvec_mul     )
    OPCODE_3( byte_sdiv,    byte, bvec_sdiv    )
    OPCODE_3( byte_udiv,    byte, bvec_udiv    )
    OPCODE_3( byte_srem,    byte, bvec_srem    )
    OPCODE_3( byte_urem,    byte, bvec_urem    )

    OPCODE_3( byte_and,     byte, bvec_and     )
    OPCODE_3( byte_or,      byte, bvec_or      )
    OPCODE_3( byte_xor,     byte, bvec_xor     )
    OPCODE_2( byte_neg,     byte, bvec_neg     )
    OPCODE_3( byte_shl,     byte, bvec_shl     )
    OPCODE_3( byte_lshr,    byte, bvec_lshr    )
    OPCODE_3( byte_ashr,    byte, bvec_ashr    )

    OPCODE_3( byte_eq,      byte, bvec_eq      )
    OPCODE_3( byte_neq,     byte, bvec_neq     )
    OPCODE_3( byte_ule,     byte, bvec_ule     )
    OPCODE_3( byte_ult,     byte, bvec_ult     )
    OPCODE_3( byte_ugt,     byte, bvec_ugt     )
    OPCODE_3( byte_uge,     byte, bvec_uge     )
    OPCODE_3( byte_sle,     byte, bvec_sle     )
    OPCODE_3( byte_slt,     byte, bvec_slt     )
    OPCODE_3( byte_sgt,     byte, bvec_sgt     )
    OPCODE_3( byte_sge,     byte, bvec_sge     )

    OPCODE_2( byte_trunc_h, byte, bvec_trunc_h )
    OPCODE_2( byte_trunc_w, byte, bvec_trunc_w )

    // half (16 bit) operations

    OPCODE_3( half_add,     half, bvec_add     )
    OPCODE_3( half_sub,     half, bvec_sub     )
    OPCODE_3( half_mul,     half, bvec_mul     )
    OPCODE_3( half_sdiv,    half, bvec_sdiv    )
    OPCODE_3( half_udiv,    half, bvec_udiv    )
    OPCODE_3( half_srem,    half, bvec_srem    )
    OPCODE_3( half_urem,    half, bvec_urem    )

    OPCODE_3( half_and,     half, bvec_and     )
    OPCODE_3( half_or,      half, bvec_or      )
    OPCODE_3( half_xor,     half, bvec_xor     )
    OPCODE_2( half_neg,     half, bvec_neg     )
    OPCODE_3( half_shl,     half, bvec_shl     )
    OPCODE_3( half_lshr,    half, bvec_lshr    )
    OPCODE_3( half_ashr,    half, bvec_ashr    )

    OPCODE_3( half_eq,      half, bvec_eq      )
    OPCODE_3( half_neq,     half, bvec_neq     )
    OPCODE_3( half_ule,     half, bvec_ule     )
    OPCODE_3( half_ult,     half, bvec_ult     )
    OPCODE_3( half_ugt,     half, bvec_ugt     )
    OPCODE_3( half_uge,     half, bvec_uge     )
    OPCODE_3( half_sle,     half, bvec_sle     )
    OPCODE_3( half_slt,     half, bvec_slt     )
    OPCODE_3( half_sgt,     half, bvec_sgt     )
    OPCODE_3( half_sge,     half, bvec_sge     )

    OPCODE_2( half_zext_b,  half, bvec_zext_b  )
    OPCODE_2( half_sext_b,  half, bvec_sext_b  )
    OPCODE_2( half_trunc_w, half, bvec_trunc_w )

    // word (32 bit) operations

    OPCODE_3( word_add,     word, bvec_add     )
    OPCODE_3( word_sub,     word, bvec_sub     )
    OPCODE_3( word_mul,     word, bvec_mul     )
    OPCODE_3( word_sdiv,    word, bvec_sdiv    )
    OPCODE_3( word_udiv,    word, bvec_udiv    )
    OPCODE_3( word_srem,    word, bvec_srem    )
    OPCODE_3( word_urem,    word, bvec_urem    )

    OPCODE_3( word_and,     word, bvec_and     )
    OPCODE_3( word_or,      word, bvec_or      )
    OPCODE_3( word_xor,     word, bvec_xor     )
    OPCODE_2( word_neg,     word, bvec_neg     )
    OPCODE_3( word_shl,     word, bvec_shl     )
    OPCODE_3( word_lshr,    word, bvec_lshr    )
    OPCODE_3( word_ashr,    word, bvec_ashr    )

    OPCODE_3( word_eq,      word, bvec_eq      )
    OPCODE_3( word_neq,     word, bvec_neq     )
    OPCODE_3( word_ule,     word, bvec_ule     )
    OPCODE_3( word_ult,     word, bvec_ult     )
    OPCODE_3( word_ugt,     word, bvec_ugt     )
    OPCODE_3( word_uge,     word, bvec_uge     )
    OPCODE_3( word_sle,     word, bvec_sle     )
    OPCODE_3( word_slt,     word, bvec_slt     )
    OPCODE_3( word_sgt,     word, bvec_sgt     )
    OPCODE_3( word_sge,     word, bvec_sge     )

    OPCODE_2( word_zext_b,  word, bvec_zext_b  )
    OPCODE_2( word_sext_b,  word, bvec_sext_b  )
    OPCODE_2( word_zext_h,  word, bvec_zext_h  )
    OPCODE_2( word_sext_h,  word, bvec_sext_h  )
    OPCODE_2( word_trunc_w, word, bvec_trunc_w )

    #undef OPCODE_2
    #undef OPCODE_3
    #undef OPCODE

} // namespace cthu

