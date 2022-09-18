#pragma once

#ifdef SSS_LUA
#include <sol/sol.hpp>
#include "SSS/Commons/color.hpp"
#include "SSS/Commons/log.hpp"

SSS_BEGIN;

template<bool b>
inline void lua_load_common_functions(sol::table_core<b>& lua) try
{
    lua.new_usertype<RGB24>("RGB",
        sol::constructors<
            RGB24(),
            RGB24(uint32_t),
            RGB24(uint8_t, uint8_t, uint8_t)
        >(),
        "rgb", &RGB24::rgb,
        "r", &RGB24::r,
        "g", &RGB24::g,
        "b", &RGB24::b
    );

    lua.new_usertype<RGBA32>("RGBA",
        sol::constructors<
            RGBA32(),
            RGBA32(RGB24, uint8_t),
            RGBA32(uint32_t),
            RGBA32(uint8_t, uint8_t, uint8_t, uint8_t)
        >(),
        "rgba", &RGBA32::rgba,
        "r", &RGBA32::r,
        "g", &RGBA32::g,
        "b", &RGBA32::b,
        "b", &RGBA32::a
    );
}
CATCH_AND_LOG_FUNC_EXC;

SSS_END;
#endif