#pragma once

#include "types.hpp"
#include "op_bvec.hpp"

namespace cthu
{
    template< builtin_type type_id >
        requires bitvec_type< type_id >::value
    struct op< exec_tag, type_id >
    {
        using value_t = typename bitvec_type< type_id >::type;

        template< typename impl_t >
        static bool binary( program &p, word opcode, impl_t impl )
        {
            value_t a = p.template pop< value_t >( bvec_arg< 0 >( opcode ) ),
                    b = p.template pop< value_t >( bvec_arg< 1 >( opcode ) );

            p.push( bvec_arg< 2 >( opcode ), value_t( impl( a, b ) ) );
            return true;
        }

        static bool apply( program &p, word opcode )
        {
            switch ( bvec_op( opcode ) )
            {
                #define BINARY( OP ) binary( p, opcode, \
                    []( value_t a, value_t b ) { return a OP b; } )

                case bvec_add<>: return BINARY( + );
                case bvec_sub<>: return BINARY( - );
                case bvec_mul<>: return BINARY( * );

               #undef BINARY

               default: UNREACHABLE( opcode, bvec_op( opcode ) );
            }
        }
    };

} // namespace cthu

