#pragma once

#include <brick-assert>

#include "build.hpp"
#include "types.hpp"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <ranges>
#include <vector>

namespace cthu
{
    struct parser
    {
        parser( std::istream &input, builder::program &prog )
            : input( input ), program( prog )
        {}

        std::vector< builder::stack_proxy > parse()
        {
            if ( !next() || !parse_structures() )
                return {};

            while ( next() && !line.ends_with( ':' ) )
            {
                line.remove_suffix( 1 );

                if ( line.starts_with( '%' ) )
                    NOT_IMPLEMENTED();
                else
                    parse_stack( std::string( line ) );
            }

            if ( !input.eof() )
                throw error( "Unexpected line" );

            if ( auto undefined = get_undefined_stacks(); undefined.empty() )
                throw error( "Stacks not declared: ", undefined );

            return {};
        }

    private:

        std::istream &input;
        builder::program &program;
        std::vector< builder::stack_proxy > init;

        std::string buffer;
        std::string_view line;
        std::size_t line_number = 1;

        std::set< std::string > structs;
        std::set< std::string > declared_stacks;
        std::map< std::string, builder::stack_proxy > stacks;

        bool parse_structures()
        {
            while ( line.starts_with( ".struct" ) )
            {
                const auto stid  = extract_word( line );
                const auto stdef = extract_word( line );

                if ( stdef != "builtin" )
                    throw error( "Unexpected structure ", stdef );

                if ( is_builtin( stid ) )
                    throw error( "Unknown builtin structure ", stid );

                if ( structs.insert( std::string( stid ) ).second )
                    throw error( "Duplicate structure definition ", stid );

                if ( !next() )
                    return false;
            }

            return true;
        }

        void parse_stack( std::string name )
        {
            if ( declared_stacks.insert( name ).second )
                throw error( "Redefinition of stack ", name );

            if ( !next() )
                return;

            auto s = program.add_stack();
            stacks.emplace( std::move( name ), s );

            while ( !line.ends_with( ':' ) )
            {
                if ( line.starts_with( '$' ) )
                    s.add( parse_number( line.substr( 1 ) ) );
                else if ( line.starts_with( '@' ) )
                    s.add( get_stack( std::string( line.substr( 1 ) ) ) );
                else
                    NOT_IMPLEMENTED();
            }
        }

        word parse_number( std::string_view str ) const
        {
            ASSERT( !str.empty() );
            ASSERT( str.find_first_not_of( '0' ) < 2 );

            word number = 0;

            for ( auto digit : str )
            {
                ASSERT( std::isdigit( digit ) );
                const word next = 10 * number + ( digit - '0' );
                if ( next < number )
                    throw error( "Invalid number - overflow" );

                number = next;
            }

            return number;
        }

        struct parse_error : std::runtime_error
        {
            using std::runtime_error::runtime_error;
        };

        template< typename... args_t >
        parse_error error( args_t... args ) const
        {
            brq::string_builder sb;
            ( sb << ... << args );
            sb << " on line " << line_number;

            return parse_error( sb.buffer() );
        }

        auto peek() const
        {
            return line;
        }

        bool next()
        {
            do
            {
                std::getline( input, buffer );
                if ( input.bad() )
                    throw error( "I/O error while reading file" );
                else if ( input.eof() )
                    return false;

                ++line_number;

                line = buffer;
                trim_left( line );
                trim_right( line );

            } while ( !line.empty() && line.starts_with( '#' ) );

            return true;
        }

        constexpr static std::string_view whitespaces = " \t\n";

        static void trim_left( std::string_view &str )
        {
            const auto start = str.find_first_not_of( whitespaces );
            if ( start != 0 && start != str.npos )
                str.remove_prefix( start );
        }

        static void trim_right( std::string_view &str )
        {
            const auto end = str.find_last_not_of( whitespaces );
            if ( end != str.size() - 1 && end != str.npos )
                str.remove_suffix( str.size() - 1 - end );
        }

        template< bool trim_left_v = true >
        static std::string_view extract_word( std::string_view &str )
        {
            if constexpr ( trim_left_v )
                trim_left( str );

            const auto pos = str.find_first_of( whitespaces );
            const auto len = pos == str.npos ? str.size() : pos;

            const auto word = str.substr( 0, len );
            str.remove_prefix( len );

            return word;
        }

        builder::stack_proxy get_stack( std::string name )
        {
            if ( auto it = stacks.find( name ); it != stacks.end() )
                return it->second;

            const auto id = program.add_stack();
            stacks.emplace( std::move( name ), id );
            return id;
        }

        std::vector< std::string > get_undefined_stacks()
        {
            std::vector< std::string > undefined;

            auto output = std::back_inserter( undefined );
            std::ranges::set_difference( std::views::keys( stacks ),
                                         declared_stacks, output );

            return undefined;
        }
    };

    auto load( std::istream &input )
    {
        builder::program prog;
        auto init = parser( input, prog ).parse();

        return prog.build( init );
    }

} // namespace cthu

