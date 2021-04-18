#pragma once

#include "SSS/Commons/_includes.hpp"
#include "SSS/Commons/time.hpp"

__SSS_BEGIN

    // --- Global static variables ---

// DEBUGMODE
#ifdef NDEBUG
  static constexpr bool DEBUGMODE = false;
#else
  static constexpr bool DEBUGMODE = true;
#endif // NDEBUG

// Error strings
namespace ERR_MSG {
    static const std::string DUPLICATE_NAME (
        "Duplicate name/path. Names & paths should be unique."
    );
    static const std::string NOTHING_FOUND (
        "Nothing was found for given argument(s)."
    );
    static const std::string INVALID_ARGUMENT (
       "Invalid argument(s)."
    );
    static const std::string OUT_OF_BOUND (
       "Invalid argument(s): Out of bound"
    );
};

    // --- Functions ---

// Converts a template to a string by outputing it to a string stream
template <typename T>
std::string toString(T const& arg) noexcept try
{
    std::ostringstream strstream;
    strstream << arg;
    return strstream.str();
}
catch (...) {
    return "[SSS::toString() error]";
}

// Adds " : " between the context, and the error message
inline std::string context_msg(std::string const& context, std::string const& msg) noexcept try
{
    return context + " : " + msg;
}
catch (...) {
    return "[SSS::context_msg() error]";
}

// Throws a runtime_error exception with given arg
__NO_RETURN void throw_exc(std::string const& str);

    // --- Log functions ---

// Logs the given argument to std::cout
void log_msg(std::string const& str) noexcept;
// Logs the given argument to std::cout
template <typename T>
inline void log_msg(T const& arg) noexcept { log_msg(toString(arg)); }

// Logs the given argument to std::cerr
void log_wrn(std::string const& str) noexcept;
// Logs the given argument to std::cerr
template <typename T>
inline void log_wrn(T const& arg) noexcept { log_wrn(toString(arg)); }

// Logs the given argument to std::cerr
void log_err(std::string const& str) noexcept;
// Logs the given argument to std::cerr
template <typename T>
inline void log_err(T const& arg) noexcept { log_err(toString(arg)); }

__SSS_END

    // --- Message macros ---

// Func macros
#define __FUNC (std::string(__func__) + "()")       // MyFunc()
#define __FUNC_MSG(X) SSS::context_msg(__FUNC, X)   // MyFunc() : Lorem ispum

// Object macros
#define __THIS_NAME (std::string(typeid(*this).name() + 6)) // MyClass
#define __THIS_ADDR std::string("0x") + SSS::toString(this) // 0x0000CFF8
#define __THIS_OBJ __THIS_NAME + " [" + __THIS_ADDR + "]"   // MyClass [0x0000CFF8]
#define __OBJ_MSG(X) SSS::context_msg(__THIS_OBJ, X)        // MyClass [0x0000CFF8] : Lorem ipsum

// Method macros
#define __METHOD (__THIS_NAME + "::" + __FUNC)          // MyClass::MyFunc()
#define __METHOD_MSG(X) SSS::context_msg(__METHOD, X)   // MyClass::MyFunc() : Lorem ispum
#define __OBJ_METHOD (__THIS_OBJ + "-> " + __FUNC)      // MyClass [0x0000CFF8]-> MyFunc()
#define __OBJ_METHOD_MSG(X) \
    SSS::context_msg(__OBJ_METHOD, X)                   // MyClass [0x0000CFF8]-> MyFunc() : Lorem Ipsum

    // --- Log macros ---

#define __LOG_MSG(X) if constexpr (SSS::DEBUGMODE) { SSS::log_msg(X); }
#define __LOG_WRN(X) if constexpr (SSS::DEBUGMODE) { SSS::log_wrn(X); }
#define __LOG_ERR(X) if constexpr (SSS::DEBUGMODE) { SSS::log_err(X); }

#define __LOG_FUNC_MSG(X) __LOG_MSG( __FUNC_MSG(X) )
#define __LOG_FUNC_ERR(X) __LOG_ERR( __FUNC_MSG(X) )
#define __LOG_FUNC_WRN(X) __LOG_WRN( __FUNC_MSG(X) )

#define __LOG_OBJ_MSG(X) __LOG_MSG( __OBJ_MSG(X) )
#define __LOG_OBJ_WRN(X) __LOG_WRN( __OBJ_MSG(X) )
#define __LOG_OBJ_ERR(X) __LOG_ERR( __OBJ_MSG(X) )

#define __LOG_METHOD_MSG(X) __LOG_MSG( __METHOD_MSG(X) )
#define __LOG_METHOD_ERR(X) __LOG_ERR( __METHOD_MSG(X) )
#define __LOG_METHOD_WRN(X) __LOG_WRN( __METHOD_MSG(X) )

#define __LOG_OBJ_METHOD_MSG(X) __LOG_MSG( __OBJ_METHOD_MSG(X) )
#define __LOG_OBJ_METHOD_ERR(X) __LOG_ERR( __OBJ_METHOD_MSG(X) )
#define __LOG_OBJ_METHOD_WRN(X) __LOG_WRN( __OBJ_METHOD_MSG(X) )

#define __LOG_CONSTRUCTOR __LOG_MSG( __OBJ_MSG("Constructor() ended") )
#define __LOG_DESTRUCTOR __LOG_MSG( __OBJ_MSG("~Destructor() ended") )

    // --- Exception macros ---

// Throws exception with object info in message
#define __OBJ_THROW(X) SSS::throw_exc(SSS::context_msg(__THIS_OBJ, X))

// Macro rethrowing exception, appending the message with the function's name
#define __CATCH_AND_RETHROW_FUNC_EXC catch (std::exception const& e) {\
    SSS::throw_exc(__FUNC_MSG(e.what()));\
}
// Macro rethrowing exception, appending the message with the method's name
#define __CATCH_AND_RETHROW_METHOD_EXC catch (std::exception const& e) {\
    SSS::throw_exc(__METHOD_MSG(e.what()));\
}

// Macro catching and logging exception with the function's infos
#define __CATCH_AND_LOG_FUNC_EXC catch (std::exception const& e) {\
    __LOG_FUNC_ERR(e.what());\
}
// Macro catching and logging exception with the method's infos
#define __CATCH_AND_LOG_METHOD_EXC catch (std::exception const& e) {\
    __LOG_OBJ_METHOD_ERR(e.what());\
}

