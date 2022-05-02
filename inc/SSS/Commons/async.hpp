#pragma once

#include "_includes.hpp"
#include "log.hpp"

/** @file
 *  Defines SSS::AsyncBase class.
 */

/** Used internally in SSS::AsyncBase.*/
#define CATCH_ASYNCBASE_ERROR \
catch (std::exception const& e) { \
    _running_state = _RunningState::handled; \
    LOG_OBJ_METHOD_CTX_ERR("Function threw", e.what()); \
};

SSS_BEGIN;

namespace Log {
    /** Logging properties for SSS::AsyncBase instances.*/
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
class AsyncBase {
public:
    /** Empty constructor.
     *  Lets you call run() whenever you want.
     */
    AsyncBase() noexcept
    {
        if (Log::Async::query(Log::Async::get().life_state)) {
            LOG_CONSTRUCTOR;
        }
    };

    /** Destructor, cancels any running async function.
     *  Calls cancel() and does not return until the async
     *  function has stopped running.
     */
    virtual ~AsyncBase() = 0;

    /** Runs user defined _asyncFunction() with given arguments.
     *  Ensures that no async function overlaps by calling
     *  cancel() beforehand.\n
     *  Automatically updates the function's state.
     *  @param[in] args The arguments to give to _asyncFunction()
     *  @sa isRunning(), isPending()
     */
    void run(_Args... args) noexcept try
    {
        cancel();
        _future = std::async(std::launch::async, &AsyncBase::_intermediateFunction, this, args...);
        _running_state = _RunningState::running;
    }
    CATCH_ASYNCBASE_ERROR;

    /** Cancels the running async function, if any.
     *  In order for this function to work as intented, you should
     *  call _beingCanceled() as often as possible in your
     *  implementation of _asyncFunction(), and return
     *  prematurely if it returns \c true.
     *  @sa run()
     */
    void cancel() noexcept try
    {
        // Return if async function was already canceled.
        if (!_future.valid()) {
            return;
        }

        // Log cancelation start
        bool const was_running = _running_state == _RunningState::running;
        if (Log::Async::query(Log::Async::get().run_state) && was_running) {
            LOG_OBJ_MSG("Canceling function ...");
        }

        // Cancel async function
        _is_canceled = true;
        _future.wait();
        _is_canceled = false;
        _running_state = _RunningState::handled;

        // Log cancelation end
        if (Log::Async::query(Log::Async::get().run_state) && was_running) {
            LOG_OBJ_MSG("Function was successfully canceled.");
        }
    }
    CATCH_ASYNCBASE_ERROR;

    /** Returns \c true if the async function is currently running.
     *  An async function can either be running, pending, or handled.
     *  @sa isPending()
     */
    inline bool isRunning() const noexcept
    {
        return _running_state == _RunningState::running;
    }
    /** Returns \c true if the async function has finished running and
     *  is waiting to be setAsHandled().
     *  An async function can either be running, pending, or handled.
     *  @sa isRunning()
     */
    inline bool isPending() const noexcept
    {
        return _running_state == _RunningState::pending;
    }
    /** Sets a pending async function's state as handled.
     *  An async function can either be running, pending, or handled.
     *  @sa isRunning(), isPending()
     */
    void setAsHandled() noexcept
    {
        if (_running_state != _RunningState::pending) {
            LOG_OBJ_WRN("Tried to set function state as 'handled' when it wasn't 'pending'");
            return;
        }
        _running_state = _RunningState::handled;
        if (_future.valid()) {
            _future.wait();
        }
        if (Log::Async::query(Log::Async::get().run_state)) {
            LOG_OBJ_MSG("Function has been handled.");
        }
    }

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
    bool _beingCanceled() const noexcept { return _is_canceled; }

private:
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
    void _intermediateFunction(_Args... args) try
    {
        if (Log::Async::query(Log::Async::get().run_state)) {
            LOG_OBJ_MSG("Function started running.");
        }

        if (_is_canceled) return;
        _asyncFunction(args...);

        if (Log::Async::query(Log::Async::get().run_state) && !_is_canceled) {
            LOG_OBJ_MSG("Function ended, now pending.");
        }
        _running_state = _RunningState::pending;
    }
    CATCH_ASYNCBASE_ERROR;
};

// Implement virtual destructor
template<class... _Args>
AsyncBase<_Args...>::~AsyncBase()
{
    cancel();

    if (Log::Async::query(Log::Async::get().life_state)) {
        LOG_DESTRUCTOR;
    }
}

SSS_END;
