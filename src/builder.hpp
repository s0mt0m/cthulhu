#pragma once

#include "program.hpp"
#include "types.hpp"

#include <map>

namespace cthu::builder
{
    struct stack
    {
        std::vector< word > data;

        std::map< word, word > stack_refs;
        const word id;

        stack( word id ) : id( id ) {}
    };

    struct program
    {
        std::vector< stack > stacks;
        word stack_id = 0;

        struct stack_proxy
        {
            program &p;
            word id;

            stack &get()
            {
                return p.stacks[ id ];
            }

            stack_proxy &add( word value )
            {
                get().data.push_back( value );
                return *this;
            }
        };

        stack_proxy add_stack()
        {
            stacks.emplace_back( stack_id++ );
            return stack_proxy{ *this, stacks.back().id };
        }

        auto build( std::vector< stack_proxy > init ) const
        {
            cthu::program p;
            std::map< word, word > program_ids;

            for ( auto &s : stacks )
            {
                auto id = p.add_stack( cthu::stack( s.data ) );
                program_ids.emplace( s.id, id );
            }

            std::vector< word > initials;

            for ( stack_proxy s : init )
                initials.push_back( program_ids[ s.id ] );

            for ( word i = init.size(); i < 8; ++i )
                initials.push_back( p.add_stack( {} ) );

            p.set_inits( initials );

            return p;
        }
    };

} // namespace cthu::builder

