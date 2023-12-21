#pragma once

#include "program.hpp"
#include "types.hpp"

#include <map>

namespace cthu::builder
{
    using word_list = std::initializer_list< word >;

    static void append_range( auto &cont, const auto &range )
    {
        cont.insert( cont.end(), range.begin(), range.end() );
    }

    struct program;
    struct stack_proxy;
    struct dictionary_proxy;

    struct stack
    {
        const word id;
        std::vector< word > data;

        std::vector< word > stack_refs;
        std::vector< word > dict_refs;

        stack( word id ) : id( id ) {}
    };

    struct stack_proxy
    {
        stack_proxy &add( word value );
        stack_proxy &add( word_list values );
        stack_proxy &add( stack_proxy stack );
        stack_proxy &add( dictionary_proxy dict );

    private:

        friend program;
        friend dictionary_proxy;

        stack_proxy( std::vector< stack > &stacks, word id )
            : stacks( stacks ), id( id )
        {}

        std::vector< stack > &stacks;
        const word id;
    };

    struct dictionary
    {
        const word id;
        std::map< word, stack_proxy > data;

        dictionary( word id ) : id( id ) {}
    };

    struct dictionary_proxy
    {
        dictionary_proxy &set( word idx, stack_proxy stack );

    private:

        friend program;
        friend stack_proxy;

        dictionary_proxy( std::vector< dictionary > &dicts, word id )
            : dicts( dicts ), id( id )
        {}

        std::vector< dictionary > &dicts;
        const word id;
    };

    struct replicator
    {
        program &p;

        replicator( auto &prog ) : p( prog ) {}

        word copy_stack( cthu::program &out, word id );
        word copy_dict( cthu::program &out, word id );
    };

    struct program
    {
        stack_proxy add_stack();
        dictionary_proxy add_dict();

        auto build( std::vector< stack_proxy > init );
        void clear() { stacks.clear(); stack_id = 0; }

    private:

        friend replicator;

        std::vector< stack > stacks;
        std::vector< dictionary > dicts;

        word stack_id = 0;
        word dict_id = 0;
    };

    auto program::build( std::vector< stack_proxy > init )
    {
        ASSERT( init.size() <= 8 );

        cthu::program p;

        return p;
    }

    word replicator::copy_stack( cthu::program &out, word id )
    {
        return 0;
    }

    word replicator::copy_dict( cthu::program &out, word id )
    {
        return 0;
    }

    stack_proxy &stack_proxy::add( word value )
    {
        stacks[ id ].data.push_back( value );
        return *this;
    }

    stack_proxy &stack_proxy::add( word_list values )
    {
        append_range( stacks[ id ].data, values );
        return *this;
    }

    stack_proxy &stack_proxy::add( stack_proxy stack )
    {
        const auto pos = stacks[ id ].data.size();
        stacks[ id ].stack_refs.emplace_back( pos );
        stacks[ id ].data.push_back( stack.id );

        return *this;
    }

    stack_proxy &stack_proxy::add( dictionary_proxy dict )
    {
        const auto pos = stacks[ id ].data.size();
        stacks[ id ].dict_refs.emplace_back( pos );
        stacks[ id ].data.push_back( dict.id );

        return *this;
    }

    dictionary_proxy &dictionary_proxy::set( word idx, stack_proxy stack )
    {
        dicts[ id ].data.emplace( idx, stack );
        return *this;
    }

    stack_proxy program::add_stack()
    {
        stacks.emplace_back( stack_id++ );
        return { stacks, stacks.back().id };
    }

    dictionary_proxy program::add_dict()
    {
        dicts.emplace_back( dict_id++ );
        return { dicts, dicts.back().id };
    }

} // namespace cthu::builder

