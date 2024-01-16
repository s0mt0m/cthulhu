#pragma once

#include "types.hpp"

namespace cthu::utils
{
    template< word count >
    constexpr word ones = ( 1u << count ) - 1;

} // namespace cthu::utils

