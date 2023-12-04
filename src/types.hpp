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

} // namespace cthu

