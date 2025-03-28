#ifndef SSS_COMMONS_ASYNC_HPP
#define SSS_COMMONS_ASYNC_HPP

#include "_includes.hpp"
#include "log.hpp"
#include "Observer.hpp"

/** @file
 *  Defines SSS::Async class.
 */

/** Used internally in SSS::Async.*/
#define CATCH_ASYNCBASE_ERROR \
catch (std::exception const& e) { \
    _running_state = _RunningState::handled; \
    LOG_CTX_ERR(CONTEXT_MSG(THIS_NAME, "Exception was caught"), e.what()); \
};

SSS_BEGIN;

namespace Log {
    /** Logging properties for SSS::Async instances.*/
    struct Async : public LogBase<Async> {
        /** \cond HIDDEN*/
        using LOG_STRUCT_BASICS(Log, Async);
        /** \endcond*/
        /** Logs both constructor and destructor.*/
        bool life_state = false;
        /** Logs when the functions starts, ends, and is handled.*/
        bool run_state = false;
    };
}

template <class...>
class Async;

SSS_COMMONS_API void pollAsync();

INTERNAL_BEGIN;

// Ignore warning about STL exports as they're private members
#pragma warning(push, 2)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

class SSS_COMMONS_API AsyncBase : public Subject {
    template <class...>
    friend class ::SSS::Async;
    friend void ::SSS::pollAsync();
public:
    using Ref = std::reference_wrapper<AsyncBase>;
private:
    static std::set<Ref> _pending;
    static std::mutex _mutex;
public:
    AsyncBase();
    AsyncBase(AsyncBase&&) = delete;
    AsyncBase(AsyncBase const&) = delete;
    virtual ~AsyncBase() = 0;

    /** Cancels the running async function, if any.
     *  In order for this function to work as intented, you should
     *  call _beingCanceled() as often as possible in your
     *  implementation of _asyncFunction(), and return
     *  prematurely if it returns \c true.
     *  @sa run()
     */
    void cancel() noexcept;


    /** Returns \c true if the async function is currently running.
     *  An async function can either be running, pending, or handled.
     */
    bool isRunning() const noexcept;

protected:
    /** To be called from your own implementation of
     *  _asyncFunction() as often as possible.
     *  In order for cancel() to work as intended, you should
     *  call this function as often as possible in your
     *  implementation of _asyncFunction(), and return
     *  prematurely if it returns \c true.
     *  @return \c true if cancel() was called but the async function
     *  is still running, and \c false otherwise.
     */
    bool _beingCanceled() const noexcept;

private:
    static void _poll() noexcept;
    void _postRun() noexcept;
    void _handle() noexcept;

    // Created in run()
    std::future<void> _future;

    // Handled by cancel()
    std::atomic<bool> _is_canceled{ false };

    enum class _RunningState {
        handled,
        running,
        pending
    };
    std::atomic<_RunningState> _running_state{ _RunningState::handled };
};

#pragma warning(pop)

INTERNAL_END;

/** Enhanced \c \b std::async class.
 * 
 *  This class aspires to render async usage easier, mainly by
 *  getting more control over the function's state with
 *  isRunning() and isPending().
 * 
 *  Set the logging properties of derived classes via SSS::Log::Async.
 * 
 *  @param[in] ..._Args All types of arguments which will be
 *  given to the run() function.
 */
template<class... _Args>
class Async : public _internal::AsyncBase{
public:
    /** Runs user defined _asyncFunction() with given arguments.
     *  Ensures that no async function overlaps by calling
     *  cancel() beforehand.\n
     *  Automatically updates the function's state.
     *  @param[in] args The arguments to give to _asyncFunction()
     *  @sa isRunning(), isPending()
     */
    void run(_Args... args) noexcept
    {
        cancel();
        _running_state = _RunningState::running;
        _future = std::async(std::launch::async, &Async::_intermediateFunction, this, args...);
    }

private:

    /** Pure virtual private function called from run():
     *  implement your own async logic in this function.
     *  In order for cancel() to work as intended, you should
     *  call _beingCanceled() as often as possible, and return
     *  prematurely if it returns \c true.
     *  @param[in] args The argument(s) of type \c _Args
     *  defined in your class declaration.
     */
    virtual void _asyncFunction(_Args... args) = 0;

    // Calls _asyncFunction and sets _running_state accordingly
    void _intermediateFunction(_Args... args) noexcept try
    {
        if (Log::Async::query(Log::Async::get().run_state)) {
            LOG_OBJ_MSG("Function started running.");
        }

        if (_is_canceled) return;
        _asyncFunction(args...);
        _postRun();
    }
    CATCH_ASYNCBASE_ERROR;
};

SSS_END;

#endif // SSS_COMMONS_ASYNC_HPP