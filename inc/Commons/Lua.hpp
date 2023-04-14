#ifndef SSS_COMMONS_LUA_HPP
#define SSS_COMMONS_LUA_HPP

#include <sol/sol.hpp>
#include "_includes.hpp"

SSS_BEGIN;

SSS_COMMONS_API void lua_setup(sol::state& lua);

SSS_END;

#endif // SSS_COMMONS_LUA_HPP