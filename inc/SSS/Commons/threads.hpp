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
        static bool cancel_state;
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
        bool const was_running = _is_running;
        if (was_running && LOG::cancel_state) {
            __LOG_OBJ_MSG("canceling thread ...");
        }

        // Cancel thread
        _is_canceled = true;
        join();
        _is_canceled = false;

        // Log cancelation end
        if (was_running && LOG::cancel_state) {
            __LOG_OBJ_MSG("thread was successfully canceled.");
        }
    };

    // Returns true if the thread is running, not to be mistaken with joinable().
    inline bool isRunning() const noexcept
    {
        return _is_running;
    }

protected:
    // Booleans
    bool _is_running{ false };  // Running state
    bool _is_canceled{ false }; // Cancelation state

    // Calls _function and sets _is_running accordingly
    void _intermediateFunction(_ProcessArgs... args) try
    {
        if (LOG::run_state) {
            __LOG_OBJ_MSG("thread started running.");
        }
        _is_running = true;
        _function(args...);
        _is_running = false;
        if (LOG::run_state) {
            __LOG_OBJ_MSG("thread ended.");
        }
    }
    catch (std::exception const& e) {
        // Thread will no longer be running
        _is_running = false;
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
template<class... _ProcessArgs>
bool ThreadBase<_ProcessArgs...>::LOG::cancel_state{ false };

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