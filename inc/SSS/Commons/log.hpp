#pragma once

#include "SSS/Commons/_includes.hpp"
#include "SSS/Commons/time.hpp"

SSS_BEGIN__

    // --- Macros ---

// DEBUGMODE
#ifdef NDEBUG
static constexpr bool DEBUGMODE = false;
#else
static constexpr bool DEBUGMODE = true;
#endif // NDEBUG
#define IF_DEBUGMODE__(X) if constexpr (DEBUGMODE) { X };

// String macros
#define FUNC__ (std::string(__func__) + "()")               // -> MyFunc()
#define CLASS__ (std::string(typeid(*this).name() + 6))     // -> MyClass
#define CLASS_ADDR__ CLASS__ + join_args(" -> 0x", this)    // -> MyClass -> 0x0000CFF8
#define METHOD__ (CLASS__ + "::" + FUNC__)                  // -> MyClass::MyFunc()

// Macro rethrowing exception, appended with the function's name
#define CATCH_AND_RETHROW_FUNC_EXC__ catch (std::exception const& e) {\
    throw_exc(get_error(FUNC__, e.what()));\
}
// Macro rethrowing exception, appended with the method's name
#define CATCH_AND_RETHROW_METHOD_EXC__ catch (std::exception const& e) {\
    throw_exc(get_error(METHOD__, e.what()));\
}

    // --- Log macros ---

#define LOG_MSG__(X) IF_DEBUGMODE__( log_msg(X); )
#define LOG_WRN__(X) IF_DEBUGMODE__( log_err(get_error("WARNING", X)); )
#define LOG_ERR__(X) IF_DEBUGMODE__( log_err(get_error("ERROR", X)); )

#define LOG_FUNC_ERR__(X) LOG_ERR__(get_error(FUNC__, X));
#define LOG_FUNC_WRN__(X) LOG_WRN__(get_error(FUNC__, X));
#define LOG_METHOD_ERR__(X) LOG_ERR__(get_error(METHOD__, X));
#define LOG_METHOD_WRN__(X) LOG_WRN__(get_error(METHOD__, X));

#define LOG_CONSTRUCTOR__ IF_DEBUGMODE__( log_msg( CLASS_ADDR__ + " -> Constructor()"); )
#define LOG_DESTRUCTOR__ IF_DEBUGMODE__( log_msg( CLASS_ADDR__ + " -> ~Destructor()"); )

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
NO_RETURN__ void throw_exc(std::string const& str);

// Throws a runtime_error exception with given arg
template <typename T>
NO_RETURN__ void throw_exc(T const& arg)
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

SSS_END__