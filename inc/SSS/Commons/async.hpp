#pragma once

#include "_includes.hpp"
#include "log.hpp"

/** @file
 *  Async logic & algorithms.
 */
__SSS_BEGIN;

/** Enhanced \c std::async class.
 *  This class aspires to render async usage easier, mainly by
 *  getting more control over the function's state with \c #isRunning()
 *  and \c #isPending().
 *  @param[in] ..._Args All types of arguments which will be given to
 *  the \c #run() function.
 */
template<class... _Args>
class AsyncBase {
public:
    /** \cond INTERNAL*/
    struct LOG {
        static bool constructor;
        static bool destructor;
        static bool run_state;
    };
    /** \endcond*/

    /** Empty constructor.
     *  Lets you call \c #run() whenever you want.
     */
    AsyncBase() noexcept
    {
        if (LOG::constructor) {
            __LOG_CONSTRUCTOR;
        }
    };

    /** Destructor, cancels any running async function.
     *  Calls \c #cancel() and does not return until the async function
     *  has stopped running.
     */
    virtual ~AsyncBase() = 0;

    /** Runs user defined \c #_asyncFunction() with given arguments.
     *  Ensures that no async function overlaps by calling \c #cancel() beforehand.\n
     *  Automatically updates the function's state.
     *  @param[in, out] args The arguments to give to \c #_asyncFunction().
     *  @sa \c #isRunning(), \c #isPending().
     */
    void run(_Args... args)
    {
        cancel();
        _future = std::async(std::launch::async, &AsyncBase::_intermediateFunction, this, args...);
    };

    /** Cancels the running async function, if any.
     *  In order for this function to work as intented, you should
     *  call \c #_beingCanceled() as often as possible in your
     *  implementation of \c #_asyncFunction(), and return
     *  prematurely if it returns \c true.
     *  @sa \c #run().
     */
    void cancel()
    {
        // Return if async function was already canceled.
        if (!_future.valid()) {
            return;
        }

        // Log cancelation start
        bool const was_running = _running_state == _RunningState::running;
        if (LOG::run_state && was_running) {
            __LOG_OBJ_MSG("Canceling function ...");
        }

        // Cancel async function
        _is_canceled = true;
        _future.wait();
        _is_canceled = false;
        _running_state = _RunningState::handled;

        // Log cancelation end
        if (LOG::run_state && was_running) {
            __LOG_OBJ_MSG("Function was successfully canceled.");
        }
    };

    /** Returns \c true if the async function is currently running.
     *  An async function can either be running, pending, or handled.
     *  @sa \c #isPending().
     */
    inline bool isRunning() const noexcept
    {
        return _running_state == _RunningState::running;
    }
    /** Returns \c true if the async function has finished running and
     *  is waiting to be \c #setAsHandled().
     *  An async function can either be running, pending, or handled.
     *  @sa \c #isRunning().
     */
    inline bool isPending() const noexcept
    {
        return _running_state == _RunningState::pending;
    }
    /** Sets a pending async function's state as handled.
     *  An async function can either be running, pending, or handled.
     *  @sa \c #isRunning(), \c #isPending().
     */
    void setAsHandled() noexcept
    {
        if (_running_state != _RunningState::pending) {
            __LOG_OBJ_WRN("Tried to set function state as 'handled' when it wasn't 'pending'");
            return;
        }
        _running_state = _RunningState::handled;
        if (_future.valid()) {
            _future.wait();
        }
        if (LOG::run_state) {
            __LOG_OBJ_MSG("Function has been handled.");
        }
    }

protected:
    /** To be called from your implementation of \c #_asyncFunction()
     *  as often as possible.
     *  In order for \c #cancel() to work as intended, you should
     *  call this function as often as possible in your
     *  implementation of \c #_asyncFunction(), and return
     *  prematurely if it returns \c true.
     *  @return \c true if \c #cancel() was called but the async function
     *  is still running, and \c false otherwise.
     */
    bool _beingCanceled() { return _is_canceled; }

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

    /** Pure virtual private function called from \c #run().
     *  Implement your own async logic in this function.\n
     *  In order for \c #cancel() to work as intended, you should
     *  call \c #_beingCanceled() as often as possible, and return
     *  prematurely if it returns \c true.
     *  @param[in,out] args The argument(s) of type \c _Args
     *  defined in your class declaration.
     */
    virtual void _asyncFunction(_Args... args) = 0;

    // Calls _asyncFunction and sets _running_state accordingly
    void _intermediateFunction(_Args... args) try
    {
        _running_state = _RunningState::running;
        if (LOG::run_state) {
            __LOG_OBJ_MSG("Function started running.");
        }

        if (_is_canceled) return;
        _asyncFunction(args...);

        if (LOG::run_state && !_is_canceled) {
            __LOG_OBJ_MSG("Function ended, now pending.");
        }
        _running_state = _RunningState::pending;
    }
#pragma warning( push )
#pragma warning( disable : 4101 )
    catch (std::exception const& e) {
        // Async function will no longer be running
        _running_state = _RunningState::handled;
        __LOG_OBJ_CTX_ERR("Function threw", e.what());
    };
#pragma warning( pop )
};

// Init static members
template<class... _Args>
bool AsyncBase<_Args...>::LOG::constructor{ false };
template<class... _Args>
bool AsyncBase<_Args...>::LOG::destructor{ false };
template<class... _Args>
bool AsyncBase<_Args...>::LOG::run_state{ false };

// Implement virtual destructor
template<class... _Args>
AsyncBase<_Args...>::~AsyncBase()
{
    cancel();

    if (LOG::destructor) {
        __LOG_DESTRUCTOR;
    }
}

__SSS_END;