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

    struct stack_builder
    {
        program &p;

        using info_t = std::tuple< ref, word, ref >;
        std::map< ref, std::vector< info_t > > blueprints;

        stack_builder( auto &prog ) : p( prog ) {}

        word create( cthu::program &out, ref id );
    };

    struct program
    {
        stack_proxy add_stack();
        dictionary_proxy add_dict();

        auto build( std::vector< stack_proxy > init );
        void clear() { stacks.clear(); stack_id = 0; }

    private:

        friend stack_builder;

        std::vector< stack > stacks;
        std::vector< stack_proxy > stack_proxies;
        std::vector< dictionary > dicts;

        word stack_id = 0;
        word dict_id = 0;
    };

    enum class color : uint8_t { WHITE, GREY, BLACK };

    static void topo_sort( ref curr, auto &stacks, auto &out, auto &marks )
    {
        if ( marks[ curr ] == color::BLACK )
            return;

        if ( marks[ curr ] == color::GREY )
            ASSERT( false ); // cycles are not allowed

        marks[ curr ] = color::GREY;

        for ( auto [ pos, next ] : stacks[ curr.ref_id ].refs )
            topo_sort( next, stacks, out, marks );

        marks[ curr ] = color::BLACK;
        out.push_back( curr );
    }

    // u -> v, v is before u
    static std::vector< ref > topo_sort( auto &stacks )
    {
        const auto count = stacks.size();

        std::vector< ref > sorted;
        sorted.reserve( count );

        std::map< ref, color > marks;
        for ( word i = 0; i < count; ++i )
            marks[ stack_ref( i ) ] = color::WHITE;

        for ( word i = 0; i < count; ++i )
            topo_sort( stack_ref( i ), stacks, sorted, marks );

        return sorted;
    }

    auto program::build( std::vector< stack_proxy > init )
    {
        ASSERT( init.size() <= 8 );

        cthu::program p;
        std::map< word, word > program_ids;

        stack_builder sb( *this );

        for ( auto i : topo_sort( stacks ) )
        {
            auto id = sb.create( p, i );
            program_ids.emplace( stacks[ i.ref_id ].id, id );
        }

        std::vector< word > initials;

        for ( stack_proxy s : init )
            initials.push_back( program_ids[ s.id ] );

        for ( word i = init.size(); i < 8; ++i )
            initials.push_back( p.add_stack( {} ) );

        p.set_inits( initials );

        return p;
    }

    word stack_builder::create( cthu::program &out, ref id )
    {
        std::vector< stack_builder::info_t > new_blueprint;

        const auto new_stack = [ this, &out ]( const auto &step )
        {
            auto [ dst, pos, src ] = step;
            const auto sid = out.add_stack( { p.stacks[ src.ref_id ].data } );
            p.stacks[ dst.ref_id ].data[ pos ] = sid;
        };

        for ( auto [ pos, ref ] : p.stacks[ id.ref_id ].refs )
        {
            ASSERT( blueprints.contains( ref ) );
            const auto &steps = blueprints[ ref ];

            for ( const auto &step : steps )
                new_stack( step );

            append_range( new_blueprint, steps );
            new_blueprint.emplace_back( id, pos, ref );

            new_stack( new_blueprint.back() );
        }

        blueprints.emplace( id, std::move( new_blueprint ) );
        return out.add_stack( { p.stacks[ id.ref_id ].data } );
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

