#pragma once

#include <thread>

#include "_includes.hpp"
#include "log.hpp"

__SSS_BEGIN

template<class... _ProcessArgs>
class ThreadBase : public std::thread {
public:

// --- Logging static variables ---

    struct LOG {
        static bool constructor;
        static bool destructor;
        static bool run_state;
    };

// --- Constructors & Destructor ---

    // Pure virtual Destructor, which is implemented outside the class declaration.
    virtual ~ThreadBase() = 0;

    // Base constructor
    ThreadBase() noexcept
    {
        if (LOG::constructor) {
            __LOG_CONSTRUCTOR
        }
    };

    // Constructor calling run(args...);
    ThreadBase(_ProcessArgs... args) try
        : ThreadBase()
    {
        run(args...);
    }
    __CATCH_AND_RETHROW_METHOD_EXC;

// --- New thread functions ---

    // Runs user defined virtual `_function()` with given args.
    void run(_ProcessArgs... args)
    {
        cancel();
        std::thread tmp(&ThreadBase::_intermediateFunction, this, args...);
        swap(tmp);
    };

    // Cancels thread execution as soon as possible.
    // The virtual _function() should often check _is_canceled
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

    // Returns true if the thread is running, not to be mistaken with joinable().
    inline bool isPending() const noexcept
    {
        return _running_state == _RunningState::pending;
    }

    // Set a pending thread's state as handled.
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
        __LOG_OBJ_MSG("thread has been handled.");
    }

protected:
    // Running state
    enum class _RunningState {
        handled,
        running,
        pending
    } _running_state{ _RunningState::handled }; 
    // Cancelation state
    bool _is_canceled{ false };

    // Calls _function and sets _running_state accordingly
    void _intermediateFunction(_ProcessArgs... args) try
    {
        _running_state = _RunningState::running;
        if (LOG::run_state) {
            __LOG_OBJ_MSG("thread started running.");
        }

        if (_is_canceled) return;
        _function(args...);

        if (LOG::run_state && !_is_canceled) {
            __LOG_OBJ_MSG("thread ended, now pending.");
        }
        _running_state = _RunningState::pending;
    }
    catch (std::exception const& e) {
        // Thread will no longer be running
        _running_state = _RunningState::handled;
        __LOG_OBJ_ERR(context_msg("thread threw", e.what()));
    };

    // User defined.
    virtual void _function(_ProcessArgs... args) = 0;

private:
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
        __LOG_DESTRUCTOR
    }
}

__SSS_END