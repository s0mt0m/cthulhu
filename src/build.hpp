#pragma once

#include "program.hpp"
#include "types.hpp"

#include <map>

namespace cthu::builder
{
    static void append_range( auto &cont, const auto &range )
    {
        cont.insert( cont.end(), range.begin(), range.end() );
    }

    struct stack
    {
        std::vector< word > data;

        std::map< word, word > stack_refs;
        const word id;

        stack( word id ) : id( id ) {}
    };

    struct program;
    struct stack_proxy;

    struct stack_builder
    {
        program &p;

        using info_t = std::tuple< word, word, word >;
        std::map< word, std::vector< info_t > > blueprints;

        stack_builder( auto &prog ) : p( prog ) {}

        word create( cthu::program &out, word id );
    };

    struct program
    {
        stack_proxy add_stack();
        auto build( std::vector< stack_proxy > init );
        void clear() { stacks.clear(); stack_id = 0; }

    private:

        friend stack_builder;
        friend stack_proxy;

        std::vector< stack > stacks;
        word stack_id = 0;
    };

    struct stack_proxy
    {
        stack_proxy &add( word value )
        {
            p.stacks[ id ].data.push_back( value );
            return *this;
        }

        stack_proxy &add( std::initializer_list< word > values )
        {
            append_range( p.stacks[ id ].data, values );
            return *this;
        }

        stack_proxy &add( stack_proxy stack_ref )
        {
            auto &s = p.stacks[ id ];

            s.stack_refs.emplace( s.data.size(), stack_ref.id );
            s.data.push_back( stack_ref.id );

            return *this;
        }

    private:

        friend program;

        stack_proxy( program &p, word id ) : p( p ), id( id ) {}

        program &p;
        word id;
    };

    stack_proxy program::add_stack()
    {
        stacks.emplace_back( stack_id++ );
        return { *this, stacks.back().id };
    }

    enum class color : uint8_t { WHITE, GREY, BLACK };

    static void topo_sort( word i, auto &stacks, auto &out, auto &marks )
    {
        if ( marks[ i ] == color::BLACK )
            return;

        if ( marks[ i ] == color::GREY )
            ASSERT( false ); // cycles are not allowed

        marks[ i ] = color::GREY;

        for ( auto [ pos, id ] : stacks[ i ].stack_refs )
            topo_sort( id, stacks, out, marks );

        marks[ i ] = color::BLACK;
        out.push_back( i );
    }

    // u -> v, v is before u
    static std::vector< word > topo_sort( auto &stacks )
    {
        const auto count = stacks.size();

        std::vector< word > sorted;
        sorted.reserve( count );

        std::vector< color > marks( count, color::WHITE );

        for ( word i = 0; i < count; ++i )
            topo_sort( i, stacks, sorted, marks );

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
            program_ids.emplace( stacks[ i ].id, id );
        }

        std::vector< word > initials;

        for ( stack_proxy s : init )
            initials.push_back( program_ids[ s.id ] );

        for ( word i = init.size(); i < 8; ++i )
            initials.push_back( p.add_stack( {} ) );

        p.set_inits( initials );

        return p;
    }

    word stack_builder::create( cthu::program &out, word id )
    {
        std::vector< stack_builder::info_t > new_blueprint;

        const auto new_stack = [ this, &out ]( const auto &step )
        {
            auto [ dst, pos, src ] = step;
            const auto sid = out.add_stack( { p.stacks[ src ].data } );
            p.stacks[ dst ].data[ pos ] = sid;
        };

        for ( auto [ pos, stack_ref ] : p.stacks[ id ].stack_refs )
        {
            ASSERT( blueprints.contains( stack_ref ) );
            const auto &steps = blueprints[ stack_ref ];

            for ( const auto &step : steps )
                new_stack( step );

            append_range( new_blueprint, steps );
            new_blueprint.emplace_back( id, pos, stack_ref );

            new_stack( new_blueprint.back() );
        }

        blueprints.emplace( id, std::move( new_blueprint ) );
        return out.add_stack( { p.stacks[ id ].data } );
    }

} // namespace cthu::builder

