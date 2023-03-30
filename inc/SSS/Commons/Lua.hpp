#ifndef SSS_COMMONS_LUA_HPP
#define SSS_COMMONS_LUA_HPP

#include <sol/sol.hpp>
#include "SSS/Commons/log.hpp"
#include "SSS/Commons/color.hpp"

SSS_BEGIN;

void lua_setup(sol::state& lua);

SSS_END;

#endif // SSS_COMMONS_LUA_HPP