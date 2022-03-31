#pragma once

#include "_includes.hpp"
#include "log.hpp"

/** @file
 *  Thread logic & algorithms.
 */
__SSS_BEGIN;

/** Enhanced \c std::thread class.
 *  This class aspires to render thread usage safer, mainly by removing
 *  \c detach() and replacing it with \c #cancel().\n
 *  You'll also get more control over the thread's state with \c #isRunning()
 *  and \c #isPending().
 *  @param[in] ..._ProcessArgs All types of arguments which will be
 *  given to the \c #run() function.
 */
template<class... _ProcessArgs>
class ThreadBase : public std::thread {
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
    ThreadBase() noexcept
    {
        if (LOG::constructor) {
            __LOG_CONSTRUCTOR;
        }
    };
    /** Constructor calling \c #run() with given arguments.
     *  Calls \c #run() right after initialisation.
     *  @param[in,out] args The arguments to give to \c #run().
     */
    ThreadBase(_ProcessArgs... args) try
        : ThreadBase()
    {
        run(args...);
    }
    __CATCH_AND_RETHROW_METHOD_EXC;

    /** Destructor, cancels any running thread.
     *  Calls \c #cancel() and does not return until the thread has stopped running.
     */
    virtual ~ThreadBase() = 0;

    /** Runs user defined \c #_threadFunction() with given arguments.
     *  Ensures that no threads overlap by calling \c #cancel() beforehand.\n
     *  Automatically updates the thread's state.
     *  @param[in, out] args The arguments to give to \c #_threadFunction().
     *  @sa \c #isRunning(), \c #isPending().
     */
    void run(_ProcessArgs... args)
    {
        cancel();
        std::thread tmp(&ThreadBase::_intermediateFunction, this, args...);
        swap(tmp);
    };

    /** Cancels the running thread, if any.
     *  In order for this function to work as intented, you should
     *  call \c #_beingCanceled() as often as possible in your
     *  implementation of \c #_threadFunction(), and return
     *  prematurely if it returns \c true.
     *  @sa \c #run().
     */
    void cancel()
    {
        // Return if thread was already joined.
        if (!joinable()) {
            return;
        }

        // Log cancelation start
        bool const was_running = _running_state == _RunningState::running;
        if (LOG::run_state && was_running) {
            __LOG_OBJ_MSG("canceling thread ...");
        }

        // Cancel thread
        _is_canceled = true;
        join();
        _is_canceled = false;
        _running_state = _RunningState::handled;

        // Log cancelation end
        if (LOG::run_state && was_running) {
            __LOG_OBJ_MSG("thread was successfully canceled.");
        }
    };

    /** Returns \c true if the thread is currently running.
     *  A thread can either be running, pending, or handled.
     *  @sa \c #isPending().
     */
    inline bool isRunning() const noexcept
    {
        return _running_state == _RunningState::running;
    }
    /** Returns \c true if the thread has finished running and
     *  is waiting to be \c #setAsHandled().
     *  A thread can either be running, pending, or handled.
     *  @sa \c #isRunning().
     */
    inline bool isPending() const noexcept
    {
        return _running_state == _RunningState::pending;
    }
    /** Sets a pending thread's state as handled.
     *  A thread can either be running, pending, or handled.
     *  @sa \c #isRunning(), \c #isPending().
     */
    void setAsHandled() noexcept
    {
        if (_running_state != _RunningState::pending) {
            __LOG_OBJ_WRN("Tried to set thread state as 'handled' when it wasn't 'pending'");
            return;
        }
        _running_state = _RunningState::handled;
        if (joinable()) {
            join();
        }
        if (LOG::run_state) {
            __LOG_OBJ_MSG("thread has been handled.");
        }
    }

protected:
    /** To be called from your implementation of \c #_threadFunction()
     *  as often as possible.
     *  In order for \c #cancel() to work as intended, you should
     *  call this function as often as possible in your
     *  implementation of \c #_threadFunction(), and return
     *  prematurely if it returns \c true.
     *  @return \c true if \c #cancel() was called but the thread
     *  is still running, and \c false otherwise.
     */
    bool _beingCanceled() { return _is_canceled; }

private:
    // Handled by cancel()
    std::atomic<bool> _is_canceled{ false };

    enum class _RunningState {
        handled,
        running,
        pending
    };
    std::atomic<_RunningState> _running_state{ _RunningState::handled };

    /** Pure virtual private function called from \c #run().
     *  Implement your own thread logic in this function.\n
     *  In order for \c #cancel() to work as intended, you should
     *  call \c #_beingCanceled() as often as possible, and return
     *  prematurely if it returns \c true.
     *  @param[in,out] args The argument(s) of type \c _ProcessArgs
     *  defined in your class declaration.
     */
    virtual void _threadFunction(_ProcessArgs... args) = 0;

    // Calls _threadFunction and sets _running_state accordingly
    void _intermediateFunction(_ProcessArgs... args) try
    {
        _running_state = _RunningState::running;
        if (LOG::run_state) {
            __LOG_OBJ_MSG("thread started running.");
        }

        if (_is_canceled) return;
        _threadFunction(args...);

        if (LOG::run_state && !_is_canceled) {
            __LOG_OBJ_MSG("thread ended, now pending.");
        }
        _running_state = _RunningState::pending;
    }
#pragma warning( push )
#pragma warning( disable : 4101 )
    catch (std::exception const& e) {
        // Thread will no longer be running
        _running_state = _RunningState::handled;
        __LOG_OBJ_ERR(context_msg("thread threw", e.what()));
    };
#pragma warning( pop )

    // Hide detach(), as it is evil and users should use cancel() instead.
    using std::thread::detach;
};

// Init static members
template<class... _ProcessArgs>
bool ThreadBase<_ProcessArgs...>::LOG::constructor{ false };
template<class... _ProcessArgs>
bool ThreadBase<_ProcessArgs...>::LOG::destructor{ false };
template<class... _ProcessArgs>
bool ThreadBase<_ProcessArgs...>::LOG::run_state{ false };

// Implement virtual destructor
template<class... _ProcessArgs>
ThreadBase<_ProcessArgs...>::~ThreadBase()
{
    cancel();

    if (LOG::destructor) {
        __LOG_DESTRUCTOR;
    }
}

__SSS_END;