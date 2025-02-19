#define SSS_LUA
#include "Commons.hpp"

class Test : public SSS::InstancedClass<Test> {
    friend SharedClass;
private:
    Test() = default;
};

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

    LOG_MSG(Test::getInstances().size());
    {
        Test::Shared test = Test::create();
        LOG_MSG(Test::get(test.get()));
        LOG_MSG(Test::getInstances().size());
    }
    LOG_MSG(Test::getInstances().size());
}
CATCH_AND_LOG_FUNC_EXC;