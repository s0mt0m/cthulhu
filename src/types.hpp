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

    template< builtin_type > struct signed_type : std::false_type {};
    template<> struct signed_type< builtin_type::byte > : std::true_type { using type = int8_t; };
    template<> struct signed_type< builtin_type::half > : std::true_type { using type = int16_t; };
    template<> struct signed_type< builtin_type::word > : std::true_type { using type = int32_t; };

    using arr_t = std::array< std::string_view, 3 >;
    static constexpr arr_t builtins{ "byte", "half", "word" };

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

