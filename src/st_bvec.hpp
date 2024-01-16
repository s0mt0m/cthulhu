#pragma once

#include "types.hpp"
#include "op_bvec.hpp"

namespace cthu
{
    template< builtin_type type_id >
        requires bitvec_type< type_id >::value
    struct op< exec_tag, type_id >
    {
        using value_t  = typename bitvec_type< type_id >::type;
        using signed_t = std::make_signed_t< value_t >;

        template< typename impl_t >
        static bool binary( program &p, word opcode, impl_t impl )
        {
            value_t a = p.template pop< value_t >( bvec_arg< 0 >( opcode ) ),
                    b = p.template pop< value_t >( bvec_arg< 1 >( opcode ) );

            p.push( bvec_arg< 2 >( opcode ), value_t( impl( a, b ) ) );
            return true;
        }

        template< typename from, typename to >
        static bool cast( program &p, word opcode )
        {
            const auto [ in, out ] = bvec_args< 2 >( opcode );

            const to value = p.template pop< from >( in );
            p.push( out, value );

            return true;
        }

        static bool apply( program &p, word opcode )
        {
            switch ( bvec_op( opcode ) )
            {
                #define BINARY_U( OP ) binary( p, opcode, \
                    []( value_t a, value_t b ) { return a OP b; } )

                #define BINARY_S( OP ) binary( p, opcode, \
                    []( signed_t a, signed_t b ) { return a OP b; } )

                case bvec_add<>:  return BINARY_U( + );
                case bvec_sub<>:  return BINARY_U( - );
                case bvec_mul<>:  return BINARY_U( * );
                case bvec_udiv<>: return BINARY_U( / );
                case bvec_urem<>: return BINARY_U( % );
                case bvec_sdiv<>: return BINARY_S( / );
                case bvec_srem<>: return BINARY_S( % );

                case bvec_and<>:  return BINARY_U( & );
                case bvec_or<>:   return BINARY_U( | );
                case bvec_xor<>:  return BINARY_U( ^ );

                case bvec_eq<>:   return BINARY_U( == );
                case bvec_neq<>:  return BINARY_U( != );
                case bvec_ult<>:  return BINARY_U( <  );
                case bvec_ule<>:  return BINARY_U( <= );
                case bvec_ugt<>:  return BINARY_U( >  );
                case bvec_uge<>:  return BINARY_U( >= );
                case bvec_slt<>:  return BINARY_S( <  );
                case bvec_sle<>:  return BINARY_S( <= );
                case bvec_sgt<>:  return BINARY_S( >  );
                case bvec_sge<>:  return BINARY_S( >= );

                #undef BINARY_U
                #undef BINARY_S


                #define CAST( FROM, TO ) cast< FROM, TO >( p, opcode )

                case bvec_zext_b<>:  return CAST( value_t,  byte );
                case bvec_zext_h<>:  return CAST( value_t,  half );

                case bvec_sext_b<>:  return CAST( signed_t, byte );
                case bvec_sext_h<>:  return CAST( signed_t, half );

                case bvec_trunc_h<>: return CAST( value_t,  half );
                case bvec_trunc_w<>: return CAST( value_t,  word );

                #undef CAST

                default: UNREACHABLE( opcode, bvec_op( opcode ) );
            }
        }
    };

} // namespace cthu

