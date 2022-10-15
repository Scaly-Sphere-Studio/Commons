#ifndef SSS_COMMONS_LUA_HPP
#define SSS_COMMONS_LUA_HPP

#ifdef SSS_LUA
#include <sol/sol.hpp>
#include "SSS/Commons/log.hpp"
#include "SSS/Commons/color.hpp"

SSS_BEGIN;

inline void lua_setup(sol::state& lua) try
{
    // Log
    {
        lua["log_msg"] = sol::overload(
            [](std::string arg) { log_msg(arg); },
            [](int64_t arg)     { log_msg(arg); },
            [](double arg)      { log_msg(arg); }
        );
        lua["log_wrn"] = sol::overload(
            [](std::string arg) { log_wrn(arg); },
            [](int64_t arg)     { log_wrn(arg); },
            [](double arg)      { log_wrn(arg); }
        );
        lua["log_err"] = sol::overload(
            [](std::string arg) { log_err(arg); },
            [](int64_t arg)     { log_err(arg); },
            [](double arg)      { log_err(arg); }
        );
    }
    // Color
    {
        // RGB24
        lua.new_usertype<RGB24>("RGB",
            sol::constructors<
                RGB24(),
                RGB24(uint32_t),
                RGB24(uint8_t, uint8_t, uint8_t)
            >(),
            "rgb", sol::property(
                [](RGB24& self) { return self.rgb; },
                [](RGB24& self, uint32_t color) { self.rgb = color; }),
            "r", &RGB24::r,
            "g", &RGB24::g,
            "b", &RGB24::b
        );
        // RGBA32
        lua.new_usertype<RGBA32>("RGBA",
            sol::constructors<
                RGBA32(),
                RGBA32(RGB24, uint8_t),
                RGBA32(uint32_t),
                RGBA32(uint8_t, uint8_t, uint8_t, uint8_t)
            >(),
            "rgba", sol::property(
                [](RGBA32& self) { return self.rgba; },
                [](RGBA32& self, uint32_t color) { self.rgba = color; }),
            "r", &RGBA32::r,
            "g", &RGBA32::g,
            "b", &RGBA32::b,
            "a", &RGBA32::a
        );
    }
}
CATCH_AND_LOG_FUNC_EXC;

SSS_END;
#endif // SSS_LUA

#endif // SSS_COMMONS_LUA_HPP