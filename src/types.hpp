#pragma once

#include <algorithm>
#include <cstdint>

namespace cthu
{
inline namespace types
{
    using byte = std::uint8_t;
    using half = std::uint16_t;
    using word = std::uint32_t;

    struct stck { word v; };
    struct dict { word v; };

} // namespace types

    enum class builtin_type : word
    {
        none,

        byte,
        half,
        word,

        stck,
        dict,

        _count_
    };

    constexpr word builtin_count = static_cast< word >( builtin_type::_count_ );

    template< typename T > constexpr word type_id {};
    template<> constexpr word type_id< void >         = word( builtin_type::none );
    template<> constexpr word type_id< types::byte >  = word( builtin_type::byte );
    template<> constexpr word type_id< types::half >  = word( builtin_type::half );
    template<> constexpr word type_id< types::word >  = word( builtin_type::word );
    template<> constexpr word type_id< types::stck >  = word( builtin_type::stck );
    template<> constexpr word type_id< types::dict >  = word( builtin_type::dict );

    template< builtin_type > struct bitvec_type : std::false_type {};
    template<> struct bitvec_type< builtin_type::byte > : std::true_type { using type = types::byte; };
    template<> struct bitvec_type< builtin_type::half > : std::true_type { using type = types::half; };
    template<> struct bitvec_type< builtin_type::word > : std::true_type { using type = types::word; };

    static constexpr std::array _builtins
    {
        "cchar",  "ucchar",
        "cshort", "ucshort",
        "cint",   "ucint",
        "clong",  "uclong",

        "cbool", "cptr",

        "conv8_8",  "conv8_16",  "conv8_32",  "conv8_64",
        "conv16_8", "conv16_16", "conv16_32", "conv16_64",
        "conv32_8", "conv32_16", "conv32_32", "conv32_64",
        "conv64_8", "conv64_16", "conv64_32", "conv64_64",

        "code", "seq",

        "func",

        "mem",
        "macc8", "macc16", "macc32", "macc64",
    };

    static constexpr std::array builtins = []()
    {
        std::array< std::string_view, _builtins.size() > arr;
        for ( std::size_t i = 0; i < arr.size(); ++i )
            arr[ i ] = _builtins[ i ];

        return arr;
    }();

    constexpr bool is_builtin( std::string_view name )
    {
        return std::ranges::find( builtins, name ) != builtins.end();
    }

    constexpr word stid_bits( word opcode ) { return opcode >> 20; }

    struct program;

    template< typename tag_t, builtin_type type >
    struct op
    {
        static bool apply( program &, word )
        {
            UNREACHABLE();
        }
    };

    struct exec_tag {};

} // namespace cthu

