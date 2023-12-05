#pragma once

#include <cstdint>

namespace cthu
{
    using byte = std::uint8_t;
    using half = std::uint16_t;
    using word = std::uint32_t;

    struct dict { word v; };
    struct stck { word v; };

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
    template<> constexpr word type_id< void >  = word( builtin_type::none );
    template<> constexpr word type_id< byte >  = word( builtin_type::byte );
    template<> constexpr word type_id< half >  = word( builtin_type::half );
    template<> constexpr word type_id< word >  = word( builtin_type::word );
    template<> constexpr word type_id< stck >  = word( builtin_type::stck );
    template<> constexpr word type_id< dict >  = word( builtin_type::dict );

    template< builtin_type > struct bitvec_type : std::false_type {};
    template<> struct bitvec_type< builtin_type::byte > : std::true_type { using type = byte; };
    template<> struct bitvec_type< builtin_type::half > : std::true_type { using type = half; };
    template<> struct bitvec_type< builtin_type::word > : std::true_type { using type = word; };

    template< builtin_type > struct signed_type : std::false_type {};
    template<> struct signed_type< builtin_type::byte > : std::true_type { using type = int8_t; };
    template<> struct signed_type< builtin_type::half > : std::true_type { using type = int16_t; };
    template<> struct signed_type< builtin_type::word > : std::true_type { using type = int32_t; };

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

