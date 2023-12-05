#pragma once

#include "program.hpp"
#include "types.hpp"

namespace cthu::builder
{
    struct stack
    {
        std::vector< word > data;

        std::map< word, word > stack_refs;
        const word id;

        stack( std::size_t id ) : id( id ) {}

        stack &add( word value )
        {
            data.push_back( value );
            return *this;
        }
    };

    struct program
    {
        std::vector< stack > stacks;
        word stack_id = 0;

        struct stack_proxy
        {
            program &p;
            word id;

            operator stack &() { return p.stacks[ id ]; }
        };

        stack_proxy add_stack()
        {
            stacks.emplace_back( stack_id++ );
            return { *this, stacks.back() };
        }

        program build( std::initializer_list< stack * > init ) const
        {
            cthu::program p;
            std::map< word, word > program_ids;

            for ( auto &s : stacks )
            {
                auto id = p.add_stack( s );
                program_ids.emplace( s.id, id );
            }

            std::vector< word > initials;

            for ( stack *sp : init )
                initials.push_back( program_ids[ sp->id ] );

            for ( word i = init.size(); i < 8; ++i )
                initials.push_back( p.add_stack( {} ) );

            p.set_inits( initials );
        }
    };

} // namespace cthu::builder

