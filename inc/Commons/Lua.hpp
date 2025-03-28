#ifndef SSS_COMMONS_LUA_HPP
#define SSS_COMMONS_LUA_HPP

#define SOL_ALL_SAFETIES_ON 1
#define SOL_STRINGS_ARE_NUMBERS 1
#include <sol/sol.hpp>
#include "log.hpp"
#include "color.hpp"

SSS_BEGIN;

inline void lua_setup(sol::state& lua) try
{
    // Log
    {
        lua["log_msg"] = sol::overload(
            sol::resolve<void(std::string const&)>(log_msg),
            sol::resolve<void(int const&)>(log_msg),
            sol::resolve<void(float const&)>(log_msg)
        );
        lua["log_wrn"] = sol::overload(
            sol::resolve<void(std::string const&)>(log_wrn),
            sol::resolve<void(int const&)>(log_wrn),
            sol::resolve<void(float const&)>(log_wrn)
        );
        lua["log_err"] = sol::overload(
            sol::resolve<void(std::string const&)>(log_err),
            sol::resolve<void(int const&)>(log_err),
            sol::resolve<void(float const&)>(log_err)
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
            // Bitfield forces this kind of sol::property
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
            "rgba", &RGBA32::rgba,
            "r", &RGBA32::r,
            "g", &RGBA32::g,
            "b", &RGBA32::b,
            "a", &RGBA32::a
        );
    }
}
CATCH_AND_LOG_FUNC_EXC;

SSS_END;

#endif // SSS_COMMONS_LUA_HPP