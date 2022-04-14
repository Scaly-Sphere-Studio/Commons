#pragma once

#include "_includes.hpp"
#include "time.hpp"
#include "conversions.hpp"

/** @file
 *  Console log & related macros, functions, constants.
 */

__SSS_BEGIN;

/** States the configuration (Release / Debug).
 *  \c true = Debug, \c false = Release.
 */
static constexpr bool DEBUGMODE
#ifdef NDEBUG
  = false;
#else
  = true;
#endif // NDEBUG

/** Holds multiple string constants used in error messages.*/
namespace ERR_MSG {
    static const std::string DUPLICATE_NAME
        = "Duplicate name/path. Names & paths should be unique.";
    static const std::string NOTHING_FOUND
        = "Nothing was found for given argument(s).";
    static const std::string INVALID_ARGUMENT
        = "Invalid argument(s).";
    static const std::string OUT_OF_BOUND
        = "Invalid argument(s): Out of bound";
};

/** Returns an error message based on an error number
 * @param[in] errnum The error number to get the error message from.
 * @return The error message generated from the error number.
 */
std::string getErrorString(int errnum);

/** Throws an \c std::runtime_error exception with given message.
 *  @param[in] str The error message to be given to the
 *  \c std::runtime_error constructor
 */
__NO_RETURN void throw_exc(std::string const& str);

/** Writes the given string to \c std::cout
 *  @param[in] str The string to write to \c std::cout
 */
void log_msg(std::string const& str) noexcept;
/** Converts the given argument of type \c T via \c SSS::toString
 *  before writing it to \c std::cout.
 *  @param[in] arg The argument of type \c T to convert and write to \c std::cout.
 */
template <typename T>
inline void log_msg(T const& arg) noexcept { log_msg(toString(arg)); }

/** Writes the given string to \c std::cerr with a warning notice.
 *  @param[in] str The string to write to \c std::cerr
 */
void log_wrn(std::string const& str) noexcept;
/** Converts the given argument of type \c T via \c SSS::toString
 *  before writing it to \c std::cerr with a warning notice.
 *  @param[in] arg The argument of type \c T to convert and write to \c std::cerr.
 */
template <typename T>
inline void log_wrn(T const& arg) noexcept { log_wrn(toString(arg)); }

/** Writes the given string to \c std::cerr with an error notice.
 *  @param[in] str The string to write to \c std::cerr.
 */
void log_err(std::string const& str) noexcept;
/** Converts the given argument of type \c T via \c SSS::toString
 *  before writing it to \c std::cerr with an error notice.
 *  @param[in] arg The argument of type \c T to convert and write to \c std::cerr.
 */
template <typename T>
inline void log_err(T const& arg) noexcept { log_err(toString(arg)); }

__SSS_END;

/** Adds ": " between the "cxt" and "msg" strings.*/
#define __CONTEXT_MSG(cxt, msg) (::SSS::toString(cxt) + ": " + ::SSS::toString(msg))

/** Current scope's function name.*/
#define __FUNC (std::string(__func__) + "()")
/** Prepends <tt>'#__FUNC: '</tt> to the given string.*/
#define __FUNC_MSG(X) __CONTEXT_MSG(__FUNC, X)

/** Current scope's class name.*/
#define __THIS_NAME (std::string(typeid(*this).name() + 6))
/** Current scope's instance address.*/
#define __THIS_ADDR std::string("0x") + ::SSS::toString(this)
/** Current scope's class name & instance address.*/
#define __THIS_OBJ __THIS_NAME + " [" + __THIS_ADDR + "]"
/** Prepends <tt>'#__THIS_OBJ: '</tt> to the given string.*/
#define __OBJ_MSG(X) __CONTEXT_MSG(__THIS_OBJ, X)

/** Current scope's method's name.*/
#define __METHOD (__THIS_NAME + "::" + __FUNC)
/** Prepends <tt>'#__METHOD: '</tt> to the given string.*/
#define __METHOD_MSG(X) __CONTEXT_MSG(__METHOD, X)
/** Current scope's instance address & method's name.*/
#define __OBJ_METHOD (__THIS_OBJ + "-> " + __FUNC)
/** Prepends <tt>'#__OBJ_METHOD: '</tt> to the given string.*/
#define __OBJ_METHOD_MSG(X) __CONTEXT_MSG(__OBJ_METHOD, X)

/** Calls \c SSS::log_msg only in debug mode.*/
#define __LOG_MSG(X)    ::SSS::log_msg( X );
/** Calls \c SSS::log_wrn only in debug mode.*/
#define __LOG_WRN(X)    ::SSS::log_wrn( X );
/** Calls \c SSS::log_err only in debug mode.*/
#define __LOG_ERR(X)    ::SSS::log_err( X );

#define __LOG_CTX_MSG(X, Y) __LOG_MSG ( __CONTEXT_MSG(X, Y) );
#define __LOG_CTX_WRN(X, Y) __LOG_ERR ( __CONTEXT_MSG(X, Y) );
#define __LOG_CTX_ERR(X, Y) __LOG_WRN ( __CONTEXT_MSG(X, Y) );

#define __LOG_FUNC_MSG(X)           __LOG_MSG       ( __FUNC_MSG(X) )
#define __LOG_FUNC_WRN(X)           __LOG_WRN       ( __FUNC_MSG(X) )
#define __LOG_FUNC_ERR(X)           __LOG_ERR       ( __FUNC_MSG(X) )
#define __LOG_FUNC_CTX_MSG(X, Y)    __LOG_CTX_MSG   ( __FUNC_MSG(X), Y )
#define __LOG_FUNC_CTX_WRN(X, Y)    __LOG_CTX_WRN   ( __FUNC_MSG(X), Y )
#define __LOG_FUNC_CTX_ERR(X, Y)    __LOG_CTX_ERR   ( __FUNC_MSG(X), Y )

#define __LOG_OBJ_MSG(X)        __LOG_MSG       ( __OBJ_MSG(X) )
#define __LOG_OBJ_WRN(X)        __LOG_WRN       ( __OBJ_MSG(X) )
#define __LOG_OBJ_ERR(X)        __LOG_ERR       ( __OBJ_MSG(X) )
#define __LOG_OBJ_CTX_MSG(X, Y) __LOG_CTX_MSG   ( __OBJ_MSG(X), Y )
#define __LOG_OBJ_CTX_WRN(X, Y) __LOG_CTX_WRN   ( __OBJ_MSG(X), Y )
#define __LOG_OBJ_CTX_ERR(X, Y) __LOG_CTX_ERR   ( __OBJ_MSG(X), Y )

#define __LOG_METHOD_MSG(X)         __LOG_MSG       ( __METHOD_MSG(X) )
#define __LOG_METHOD_WRN(X)         __LOG_WRN       ( __METHOD_MSG(X) )
#define __LOG_METHOD_ERR(X)         __LOG_ERR       ( __METHOD_MSG(X) )
#define __LOG_METHOD_CTX_MSG(X, Y)  __LOG_CTX_MSG   ( __METHOD_MSG(X), Y )
#define __LOG_METHOD_CTX_WRN(X, Y)  __LOG_CTX_WRN   ( __METHOD_MSG(X), Y )
#define __LOG_METHOD_CTX_ERR(X, Y)  __LOG_CTX_ERR   ( __METHOD_MSG(X), Y )

#define __LOG_OBJ_METHOD_MSG(X)         __LOG_MSG       ( __OBJ_METHOD_MSG(X) )
#define __LOG_OBJ_METHOD_WRN(X)         __LOG_WRN       ( __OBJ_METHOD_MSG(X) )
#define __LOG_OBJ_METHOD_ERR(X)         __LOG_ERR       ( __OBJ_METHOD_MSG(X) )
#define __LOG_OBJ_METHOD_CTX_MSG(X, Y)  __LOG_CTX_MSG   ( __OBJ_METHOD_MSG(X), Y )
#define __LOG_OBJ_METHOD_CTX_WRN(X, Y)  __LOG_CTX_WRN   ( __OBJ_METHOD_MSG(X), Y )
#define __LOG_OBJ_METHOD_CTX_ERR(X, Y)  __LOG_CTX_ERR   ( __OBJ_METHOD_MSG(X), Y )


/** Logs a constructor notice using \c #__OBJ_MSG and a preset message.*/
#define __LOG_CONSTRUCTOR __LOG_MSG( __OBJ_MSG("Constructor() ended") )
/** Logs a destructor notice using \c #__OBJ_MSG and a preset message.*/
#define __LOG_DESTRUCTOR __LOG_MSG( __OBJ_MSG("~Destructor() ended") )

/** Throws an exception with \c #__THIS_OBJ prepended to given message.*/
#define __OBJ_THROW(X) ::SSS::throw_exc(__CONTEXT_MSG(__THIS_OBJ, X))

/** Catches exceptions, prepends the function's name to error messages, and rethrows.*/
#define __CATCH_AND_RETHROW_FUNC_EXC \
catch (std::exception const& e) {\
    ::SSS::throw_exc(__FUNC_MSG(e.what()));\
}
/** Catches exceptions, prepends the method's name to error messages, and rethrows.*/
#define __CATCH_AND_RETHROW_METHOD_EXC \
catch (std::exception const& e) {\
    ::SSS::throw_exc(__METHOD_MSG(e.what()));\
}

/** Catches exceptions & prepends the function's name to error messages before logging them.*/
#define __CATCH_AND_LOG_FUNC_EXC \
catch (std::exception const& e) {\
    __LOG_FUNC_ERR(e.what());\
}
/** Catches exceptions & prepends the method's name to error messages before logging them.*/
#define __CATCH_AND_LOG_METHOD_EXC \
catch (std::exception const& e) {\
    __LOG_OBJ_METHOD_ERR(e.what());\
}
