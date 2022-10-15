#ifndef SSS_COMMONS_LOG_HPP
#define SSS_COMMONS_LOG_HPP

#include "_includes.hpp"
#include "time.hpp"
#include "conversions.hpp"

/** @file
 *  Defines log functions, constants, template class, and numerous macros.
 */

SSS_BEGIN;

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

/** Singleton template class to be inherited by log structures.
 *  %Log structures are intended to be nested in the Log (or
 *  subsequent) namespace and access its data with the
 *  declaration: <tt>using #LOG_STRUCT_BASICS()</tt>.
 *  @param[in] T The derived struct itself.
 *  @usage
 *  @code
 *  namespace SSS::Log {
 *      struct Example : SSS::LogBase<Example> {
 *          using LOG_STRUCT_BASICS(Log, Example);
 *          bool foo = false;
 *          bool bar = false;
 *      }
 *  }
 *  @endcode
 */
template<typename T>
class LogBase {
protected:
    /** \cond INTERNAL*/
    // Hide constructor & destructor
    LogBase() = default;
    ~LogBase() = default;
    // Share declarations between derived classes without sharing actual data 
    bool _silence = false;
    bool _louden = false;
    /** \endcond*/
public:
    /** Returns the singleton instance (lazy instantiation pattern).*/
    static T& get() {
        static T instance;
        return instance;
    };
    /** Sets the silenced mode state to \c true or \c false.
     *  In silenced mode, all subsequent log flags act as if
     *  they were set to \c false without actually changing
     *  their value.
     */
    static void silence(bool state) { get()._silence = state; };
    /** Sets the loudened mode state to \c true or \c false.
     *  In loudened mode, all subsequent log flags act as if
     *  they were set to \c true without actually changing
     *  their value.
     */
    static void louden(bool state) { get()._louden = state; };
    /** Returns \c true if silenced mode is active in any parent
     *  namespace or in this structure, and \c false otherwise.
     */
    static bool isSilenced() { return get()._silence; };
    /** Returns \c true if loudened mode is active in any parent
     *  namespace or in this structure, and \c false otherwise.
     */
    static bool isLoudened() { return get()._louden; };
    /** Returns \c false if silenced mode is active, \c true if
     *  loudened mode is active, and the given flag otherwise.
     */
    static bool query(bool flag) { return !isSilenced() && (isLoudened() || flag); };
};

/** Base namespace for all subsequent log namespaces and structures.
 *  Holds an internal LogBase instance and handles to its functions,
 *  as if #LOG_NAMESPACE_BASICS was used without specifying a namespace.
 * 
 *  Subsequent namespaces should be declared as such:
 *  @code
 *  namespace SSS::Log::Example {
 *      LOG_NAMESPACE_BASICS(SSS::Log);
 *      [...]
 *  }
 *  @endcode
 */
namespace Log {
    INTERNAL_BEGIN;
    struct Base : public LogBase<Base> {
        friend LogBase<Base>;
    private:
        Base() = default;
        ~Base() = default;
    };
    INTERNAL_END;
    /** Handle to the internal instance's LogBase::louden() function.*/
    inline void louden(bool state) { _internal::Base::louden(state); };
    /** Handle to the internal instance's LogBase::silence() function.*/
    inline void silence(bool state) { _internal::Base::silence(state); };
    /** Handle to the internal instance's LogBase::isLoudened() function.*/
    inline bool isLoudened() { return _internal::Base::isLoudened(); };
    /** Handle to the internal instance's LogBase::isSilenced() function.*/
    inline bool isSilenced() { return _internal::Base::isSilenced(); };
}

SSS_END;

/** To be included in SSS::LogBase derived classes.
 *  Hides constructor and destructor to enforce singleton pattern.\n
 *  Declares parent class as friend to allow constructor to be called.\n
 *  Re-defines namespace dependent functions.
 *  @param Namespace The namespace the struct is being nested into.
 *  @param Struct The struct name itself.
 */
#define LOG_STRUCT_BASICS(Namespace, Struct)\
    SSS::LogBase<Struct>::LogBase;\
    friend SSS::LogBase<Struct>;\
private:\
    Struct() = default;\
    ~Struct() = default;\
public:\
    static bool isSilenced() { return Namespace::isSilenced() || get()._silence; };\
    static bool isLoudened() { return Namespace::isLoudened() || get()._louden; };\
    static bool query(bool flag) { return !isSilenced() && (isLoudened() || flag); };

/** To be included in any subsequent SSS::Log namespace.
 *  Defines internal SSS::LogBase instance and handles to its functions.
 *  @param Namespace The namespace from which this one is being nested into.
 */
#define LOG_NAMESPACE_BASICS(Namespace)\
INTERNAL_BEGIN;\
struct Base : public SSS::LogBase<Base> {\
    using LOG_STRUCT_BASICS(Namespace, Base);\
};\
INTERNAL_END;\
inline void silence(bool state) { _internal::Base::silence(state); };\
inline void louden(bool state) { _internal::Base::louden(state); };\
inline bool isSilenced() { return _internal::Base::isSilenced(); };\
inline bool isLoudened() { return _internal::Base::isLoudened(); };

/** Adds ": " between the "cxt" and "msg" strings.*/
#define CONTEXT_MSG(cxt, msg) (SSS::toString(cxt) + ": " + SSS::toString(msg))

/** Current scope's function name.*/
#define FUNC (std::string(__func__) + "()")
/** Prepends <tt>'#FUNC: '</tt> to the given string.*/
#define FUNC_MSG(X) CONTEXT_MSG(FUNC, X)

/** Current scope's class name.*/
#define THIS_NAME (std::string(typeid(*this).name() + 6))
/** Current scope's instance address.*/
#define THIS_ADDR std::string("0x") + SSS::toString(this)
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
#define LOG_MSG(X)    SSS::log_msg( X );
/** Calls \c SSS::log_wrn only in debug mode.*/
#define LOG_WRN(X)    SSS::log_wrn( X );
/** Calls \c SSS::log_err only in debug mode.*/
#define LOG_ERR(X)    SSS::log_err( X );

#define LOG_CTX_MSG(X, Y)   LOG_MSG ( CONTEXT_MSG(X, Y) );
#define LOG_CTX_WRN(X, Y)   LOG_WRN ( CONTEXT_MSG(X, Y) );
#define LOG_CTX_ERR(X, Y)   LOG_ERR ( CONTEXT_MSG(X, Y) );

#define LOG_FUNC_MSG(X)         LOG_MSG     ( FUNC_MSG(X) )
#define LOG_FUNC_WRN(X)         LOG_WRN     ( FUNC_MSG(X) )
#define LOG_FUNC_ERR(X)         LOG_ERR     ( FUNC_MSG(X) )
#define LOG_FUNC_CTX_MSG(X, Y)  LOG_CTX_MSG ( FUNC_MSG(X), Y )
#define LOG_FUNC_CTX_WRN(X, Y)  LOG_CTX_WRN ( FUNC_MSG(X), Y )
#define LOG_FUNC_CTX_ERR(X, Y)  LOG_CTX_ERR ( FUNC_MSG(X), Y )

#define LOG_OBJ_MSG(X)          LOG_MSG     ( OBJ_MSG(X) )
#define LOG_OBJ_WRN(X)          LOG_WRN     ( OBJ_MSG(X) )
#define LOG_OBJ_ERR(X)          LOG_ERR     ( OBJ_MSG(X) )
#define LOG_OBJ_CTX_MSG(X, Y)   LOG_CTX_MSG ( OBJ_MSG(X), Y )
#define LOG_OBJ_CTX_WRN(X, Y)   LOG_CTX_WRN ( OBJ_MSG(X), Y )
#define LOG_OBJ_CTX_ERR(X, Y)   LOG_CTX_ERR ( OBJ_MSG(X), Y )

#define LOG_METHOD_MSG(X)           LOG_MSG     ( METHOD_MSG(X) )
#define LOG_METHOD_WRN(X)           LOG_WRN     ( METHOD_MSG(X) )
#define LOG_METHOD_ERR(X)           LOG_ERR     ( METHOD_MSG(X) )
#define LOG_METHOD_CTX_MSG(X, Y)    LOG_CTX_MSG ( METHOD_MSG(X), Y )
#define LOG_METHOD_CTX_WRN(X, Y)    LOG_CTX_WRN ( METHOD_MSG(X), Y )
#define LOG_METHOD_CTX_ERR(X, Y)    LOG_CTX_ERR ( METHOD_MSG(X), Y )

#define LOG_OBJ_METHOD_MSG(X)           LOG_MSG     ( OBJ_METHOD_MSG(X) )
#define LOG_OBJ_METHOD_WRN(X)           LOG_WRN     ( OBJ_METHOD_MSG(X) )
#define LOG_OBJ_METHOD_ERR(X)           LOG_ERR     ( OBJ_METHOD_MSG(X) )
#define LOG_OBJ_METHOD_CTX_MSG(X, Y)    LOG_CTX_MSG ( OBJ_METHOD_MSG(X), Y )
#define LOG_OBJ_METHOD_CTX_WRN(X, Y)    LOG_CTX_WRN ( OBJ_METHOD_MSG(X), Y )
#define LOG_OBJ_METHOD_CTX_ERR(X, Y)    LOG_CTX_ERR ( OBJ_METHOD_MSG(X), Y )


/** Logs a constructor notice using \c #OBJ_MSG and a preset message.*/
#define LOG_CONSTRUCTOR LOG_MSG( OBJ_MSG("Constructor() ended") )
/** Logs a destructor notice using \c #OBJ_MSG and a preset message.*/
#define LOG_DESTRUCTOR LOG_MSG( OBJ_MSG("~Destructor() ended") )

/** Throws an exception with \c #THIS_OBJ prepended to given message.*/
#define OBJ_THROW(X) SSS::throw_exc(CONTEXT_MSG(THIS_OBJ, X))

/** Catches exceptions, prepends the function's name to error messages, and rethrows.*/
#define CATCH_AND_RETHROW_FUNC_EXC \
catch (std::exception const& e) {\
    SSS::throw_exc(FUNC_MSG(e.what()));\
}
/** Catches exceptions, prepends the method's name to error messages, and rethrows.*/
#define CATCH_AND_RETHROW_METHOD_EXC \
catch (std::exception const& e) {\
    SSS::throw_exc(METHOD_MSG(e.what()));\
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

#endif // SSS_COMMONS_LOG_HPP