#define SSS_LUA
#include "Commons.hpp"

int main(void) try
{
    sol::state lua;
    SSS::lua_setup(lua);

    lua.script(R"lua(
col = RGBA.new(255, 0, 255, 255)
log_msg(col.r)
log_msg(col.g)
log_msg(col.b)
log_msg(col.a)
)lua");

}
CATCH_AND_LOG_FUNC_EXC;