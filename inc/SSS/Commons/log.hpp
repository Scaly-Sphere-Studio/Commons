#pragma once

#include "SSS/Commons/_includes.hpp"
#include "SSS/Commons/time.hpp"

__SSS_BEGIN

    // --- Macros ---

// DEBUGMODE
#ifdef NDEBUG
static constexpr bool DEBUGMODE = false;
#else
static constexpr bool DEBUGMODE = true;
#endif // NDEBUG
#define __IF_DEBUGMODE(X) if constexpr (DEBUGMODE) { X };

// String macros
#define __FUNC (std::string(__func__) + "()")               // -> MyFunc()
#define __CLASS (std::string(typeid(*this).name() + 6))     // -> MyClass
#define __CLASS_ADDR __CLASS + join_args(" -> 0x", this)    // -> MyClass -> 0x0000CFF8
#define __METHOD (__CLASS + "::" + __FUNC)                  // -> MyClass::MyFunc()

// Macro rethrowing exception, appended with the function's name
#define __CATCH_AND_RETHROW_FUNC_EXC catch (std::exception const& e) {\
    throw_exc(get_error(__FUNC, e.what()));\
}
// Macro rethrowing exception, appended with the method's name
#define __CATCH_AND_RETHROW_METHOD_EXC catch (std::exception const& e) {\
    throw_exc(get_error(__METHOD, e.what()));\
}

    // --- Log macros ---

#define __LOG_MSG(X) __IF_DEBUGMODE( log_msg(X); )
#define __LOG_WRN(X) __IF_DEBUGMODE( log_err(get_error("WARNING", X)); )
#define __LOG_ERR(X) __IF_DEBUGMODE( log_err(get_error("ERROR", X)); )

#define __LOG_FUNC_ERR(X) __LOG_ERR(get_error(__FUNC, X));
#define __LOG_FUNC_WRN(X) __LOG_WRN(get_error(__FUNC, X));
#define __LOG_METHOD_ERR(X) __LOG_ERR(get_error(__METHOD, X));
#define __LOG_METHOD_WRN(X) __LOG_WRN(get_error(__METHOD, X));

#define __LOG_CONSTRUCTOR __IF_DEBUGMODE( log_msg( __CLASS_ADDR + " -> Constructor()"); )
#define __LOG_DESTRUCTOR __IF_DEBUGMODE( log_msg( __CLASS_ADDR + " -> ~Destructor()"); )

    // --- Error strings ---

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
    static const std::string LOG_ERROR (
       "Error in log output. Users shouldn't see this."
    );
};

    // --- Static Functions ---

// Static function that converts a template to a string
template <typename T>
static std::string template_to_string_(T const& arg) noexcept try
{
    std::ostringstream strstream;
    strstream << arg;
    return strstream.str();
}
catch (...) {
    return ERR_MSG::LOG_ERROR;
}

    // --- Functions ---

// Logs the given argument to std::cout
void log_msg(std::string const& str) noexcept;

// Logs the given argument to std::cout
template <typename T>
void log_msg(T const& arg) noexcept
{
    log_msg(template_to_string_(arg));
}

// Logs the given argument to std::cerr
void log_err(std::string const& str) noexcept;

// Logs the given argument to std::cerr
template <typename T>
void log_err(T const& arg) noexcept
{
    log_err(template_to_string_(arg));
}

// Throws a runtime_error exception with given arg
__NO_RETURN void throw_exc(std::string const& str);

// Throws a runtime_error exception with given arg
template <typename T>
__NO_RETURN void throw_exc(T const& arg)
{
    throw_exc(template_to_string_(arg));
}

// Adds " : " between the function's name, and the argument
template <typename T>
std::string get_error(std::string const& funcname, T const& arg) noexcept
{
    return funcname + " : " + template_to_string_(arg);
}

// Joins any arguments together as strings
template <typename T, typename T2>
std::string join_args(T const& arg, T2 const& msg) noexcept
{
    return template_to_string_(arg) + template_to_string_(msg);
}

__SSS_END