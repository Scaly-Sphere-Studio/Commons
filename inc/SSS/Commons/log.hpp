#pragma once

#include "_includes.hpp"
#include "time.hpp"
#include "conversions.hpp"

/** @file
 *  Defines log functions, constants, and numerous macros.
 */

SSS_BEGIN;

/** States the configuration (Release / Debug).
 *  \c true = Debug, \c false = Release.
 */
extern constexpr bool DEBUGMODE
/** \cond SKIP*/
#ifdef NDEBUG
  = false
#else
  = true
#endif // NDEBUG
/** \endcond*/
;

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

/** Returns an error message based on given error number
 * @param[in] errnum The error number to generate the error message from.
 * @return The error message generated from the error number.
 */
std::string getErrorString(int errnum);

/** Throws a \c std::runtime_error exception with given message.
 *  @param[in] str The error message to be given to the
 *  \c std::runtime_error constructor
 */
NO_RETURN void throw_exc(std::string const& str);

SSS_END;

/** Adds ": " between the "cxt" and "msg" strings.*/
#define CONTEXT_MSG(cxt, msg) (::SSS::toString(cxt) + ": " + ::SSS::toString(msg))

/** Current scope's function name.*/
#define FUNC (std::string(func) + "()")
/** Prepends <tt>'#FUNC: '</tt> to the given string.*/
#define FUNC_MSG(X) CONTEXT_MSG(FUNC, X)

/** Current scope's class name.*/
#define THIS_NAME (std::string(typeid(*this).name() + 6))
/** Current scope's instance address.*/
#define THIS_ADDR std::string("0x") + ::SSS::toString(this)
/** Current scope's class name & instance address.*/
#define THIS_OBJ THIS_NAME + " [" + THIS_ADDR + "]"
/** Prepends <tt>'#THIS_OBJ: '</tt> to the given string.*/
#define OBJ_MSG(X) CONTEXT_MSG(THIS_OBJ, X)

/** Current scope's method's name.*/
#define METHOD (THIS_NAME + "::" + FUNC)
/** Prepends <tt>'#METHOD: '</tt> to the given string.*/
#define METHOD_MSG(X) CONTEXT_MSG(METHOD, X)
/** Current scope's instance address & method's name.*/
#define OBJ_METHOD (THIS_OBJ + "-> " + FUNC)
/** Prepends <tt>'#OBJ_METHOD: '</tt> to the given string.*/
#define OBJ_METHOD_MSG(X) CONTEXT_MSG(OBJ_METHOD, X)

/** Calls \c SSS::log_msg only in debug mode.*/
#define LOG_MSG(X)    ::SSS::log_msg( X );
/** Calls \c SSS::log_wrn only in debug mode.*/
#define LOG_WRN(X)    ::SSS::log_wrn( X );
/** Calls \c SSS::log_err only in debug mode.*/
#define LOG_ERR(X)    ::SSS::log_err( X );

#define LOG_CTX_MSG(X, Y) LOG_MSG ( CONTEXT_MSG(X, Y) );
#define LOG_CTX_WRN(X, Y) LOG_ERR ( CONTEXT_MSG(X, Y) );
#define LOG_CTX_ERR(X, Y) LOG_WRN ( CONTEXT_MSG(X, Y) );

#define LOG_FUNC_MSG(X)           LOG_MSG       ( FUNC_MSG(X) )
#define LOG_FUNC_WRN(X)           LOG_WRN       ( FUNC_MSG(X) )
#define LOG_FUNC_ERR(X)           LOG_ERR       ( FUNC_MSG(X) )
#define LOG_FUNC_CTX_MSG(X, Y)    LOG_CTX_MSG   ( FUNC_MSG(X), Y )
#define LOG_FUNC_CTX_WRN(X, Y)    LOG_CTX_WRN   ( FUNC_MSG(X), Y )
#define LOG_FUNC_CTX_ERR(X, Y)    LOG_CTX_ERR   ( FUNC_MSG(X), Y )

#define LOG_OBJ_MSG(X)        LOG_MSG       ( OBJ_MSG(X) )
#define LOG_OBJ_WRN(X)        LOG_WRN       ( OBJ_MSG(X) )
#define LOG_OBJ_ERR(X)        LOG_ERR       ( OBJ_MSG(X) )
#define LOG_OBJ_CTX_MSG(X, Y) LOG_CTX_MSG   ( OBJ_MSG(X), Y )
#define LOG_OBJ_CTX_WRN(X, Y) LOG_CTX_WRN   ( OBJ_MSG(X), Y )
#define LOG_OBJ_CTX_ERR(X, Y) LOG_CTX_ERR   ( OBJ_MSG(X), Y )

#define LOG_METHOD_MSG(X)         LOG_MSG       ( METHOD_MSG(X) )
#define LOG_METHOD_WRN(X)         LOG_WRN       ( METHOD_MSG(X) )
#define LOG_METHOD_ERR(X)         LOG_ERR       ( METHOD_MSG(X) )
#define LOG_METHOD_CTX_MSG(X, Y)  LOG_CTX_MSG   ( METHOD_MSG(X), Y )
#define LOG_METHOD_CTX_WRN(X, Y)  LOG_CTX_WRN   ( METHOD_MSG(X), Y )
#define LOG_METHOD_CTX_ERR(X, Y)  LOG_CTX_ERR   ( METHOD_MSG(X), Y )

#define LOG_OBJ_METHOD_MSG(X)         LOG_MSG       ( OBJ_METHOD_MSG(X) )
#define LOG_OBJ_METHOD_WRN(X)         LOG_WRN       ( OBJ_METHOD_MSG(X) )
#define LOG_OBJ_METHOD_ERR(X)         LOG_ERR       ( OBJ_METHOD_MSG(X) )
#define LOG_OBJ_METHOD_CTX_MSG(X, Y)  LOG_CTX_MSG   ( OBJ_METHOD_MSG(X), Y )
#define LOG_OBJ_METHOD_CTX_WRN(X, Y)  LOG_CTX_WRN   ( OBJ_METHOD_MSG(X), Y )
#define LOG_OBJ_METHOD_CTX_ERR(X, Y)  LOG_CTX_ERR   ( OBJ_METHOD_MSG(X), Y )


/** Logs a constructor notice using \c #OBJ_MSG and a preset message.*/
#define LOG_CONSTRUCTOR LOG_MSG( OBJ_MSG("Constructor() ended") )
/** Logs a destructor notice using \c #OBJ_MSG and a preset message.*/
#define LOG_DESTRUCTOR LOG_MSG( OBJ_MSG("~Destructor() ended") )

/** Throws an exception with \c #THIS_OBJ prepended to given message.*/
#define OBJ_THROW(X) ::SSS::throw_exc(CONTEXT_MSG(THIS_OBJ, X))

/** Catches exceptions, prepends the function's name to error messages, and rethrows.*/
#define CATCH_AND_RETHROW_FUNC_EXC \
catch (std::exception const& e) {\
    ::SSS::throw_exc(FUNC_MSG(e.what()));\
}
/** Catches exceptions, prepends the method's name to error messages, and rethrows.*/
#define CATCH_AND_RETHROW_METHOD_EXC \
catch (std::exception const& e) {\
    ::SSS::throw_exc(METHOD_MSG(e.what()));\
}

/** Catches exceptions & prepends the function's name to error messages before logging them.*/
#define CATCH_AND_LOG_FUNC_EXC \
catch (std::exception const& e) {\
    LOG_FUNC_ERR(e.what());\
}
/** Catches exceptions & prepends the method's name to error messages before logging them.*/
#define CATCH_AND_LOG_METHOD_EXC \
catch (std::exception const& e) {\
    LOG_OBJ_METHOD_ERR(e.what());\
}
