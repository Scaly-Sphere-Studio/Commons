#pragma once

#include "_includes.hpp"

// SFINAE
template<typename Enum>
struct _EnableBitMaskOperators {
    static const bool enable = false;
};

// SFINAE
#define __ENABLE_BITMASK_OPERATORS(x)   \
template<>                              \
struct ::_EnableBitMaskOperators<x> {    \
    static const bool enable = true;    \
};

template<typename Enum>
typename std::enable_if<_EnableBitMaskOperators<Enum>::enable, Enum>::type
operator| (Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<Enum> (
        static_cast<underlying>(lhs)
        | static_cast<underlying>(rhs)
    );
}

template<typename Enum>
typename std::enable_if<_EnableBitMaskOperators<Enum>::enable, Enum>::type
operator& (Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<Enum> (
        static_cast<underlying>(lhs)
        & static_cast<underlying>(rhs)
    );
}

// Returns true if the given enum is set to `None`, and false otherwise.
// The given enum class must have an enumerator called `None` for this code to compile.
template<typename Enum>
typename std::enable_if<_EnableBitMaskOperators<Enum>::enable, bool>::type
operator!(Enum e)
{
    return e == Enum::None;
}