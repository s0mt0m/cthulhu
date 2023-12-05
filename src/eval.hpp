#pragma once

#include "types.hpp"

namespace cthu
{
    template< typename tag_t, word static_type = builtin_count - 1 >
    bool type_dispatch( word type, program &prog, word opcode )
    {
        ASSERT( type <= static_type );

        using operation = op< tag_t, builtin_type( static_type ) >;

        if ( type == static_type )
            return operation::apply( prog, opcode );

        if constexpr ( static_type > 0 )
        {
            constexpr auto dispatch = type_dispatch< tag_t, static_type - 1 >;
            return dispatch( type, prog, opcode );
        }

        UNREACHABLE();
    }

    template< typename tag_t >
    struct eval
    {
        program &prog;

        eval( program &p ) : prog( p ) {}

        bool operator()()
        {
            while ( !prog.halted() )
            {
                const auto opcode = prog.next_instruction();
                const auto stid = stid_bits( opcode );

                if ( !type_dispatch< tag_t >( stid, prog, opcode ) )
                    return false;
            }

            return true;
        }
    };

    using exec = eval< exec_tag >;

} // namespace cthu

