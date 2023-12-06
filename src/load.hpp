#pragma once

#include "build.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace cthu
{
    struct parser
    {
        parser( std::istream &input, builder::program &prog )
            : input( input ), program( prog )
        {}

        std::vector< builder::stack_proxy > parse()
        {
            return {};
        }

    private:

        std::istream &input;
        builder::program &program;
        std::vector< builder::stack_proxy > init;

        std::string buffer;
        std::string_view line;
        std::size_t line_number = 1;

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
    };

    auto load( std::istream &input )
    {
        builder::program prog;
        auto init = parser( input, prog ).parse();

        return prog.build( init );
    }

} // namespace cthu

