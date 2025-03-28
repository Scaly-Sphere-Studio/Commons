#include "Commons/Async.hpp"

SSS_BEGIN;

void pollAsync()
{
    _internal::AsyncBase::_poll();
}

INTERNAL_BEGIN;

std::set<AsyncBase::Ref> AsyncBase::_pending;
std::mutex AsyncBase::_mutex;

std::strong_ordering operator<=>(AsyncBase::Ref const& a, AsyncBase::Ref const& b)
{
    return &a.get() <=> &b.get();
}

AsyncBase::AsyncBase()
{
    if (Log::Async::query(Log::Async::get().life_state)) {
        LOG_CONSTRUCTOR;
    }
}

AsyncBase::~AsyncBase()
{
    cancel();

    {
        std::unique_lock const lock(_mutex);
        _pending.erase(*this);
    }

    if (Log::Async::query(Log::Async::get().life_state)) {
        LOG_DESTRUCTOR;
    }
}

void AsyncBase::cancel() noexcept try
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

bool AsyncBase::isRunning() const noexcept
{
    return _running_state == _RunningState::running;
}

bool AsyncBase::_beingCanceled() const noexcept
{
    return _is_canceled;
}

void AsyncBase::_poll() noexcept
{
    std::unique_lock const lock(_mutex);
    for (auto&& ref : _pending)
        ref.get()._handle();
    _pending.clear();
}

void AsyncBase::_postRun() noexcept
{
    if (Log::Async::query(Log::Async::get().run_state) && !_is_canceled) {
        LOG_OBJ_MSG("Function ended, now pending.");
    }
    _running_state = _RunningState::pending;

    std::unique_lock const lock(_mutex);
    _pending.emplace(*this);
}

void AsyncBase::_handle() noexcept
{
    if (_running_state != _RunningState::pending) {
        LOG_OBJ_WRN("Tried to set function state as 'handled' when it wasn't 'pending'");
        return;
    }

    _notifyObservers();
    
    _running_state = _RunningState::handled;
    if (_future.valid()) {
        _future.wait();
    }

    if (Log::Async::query(Log::Async::get().run_state)) {
        LOG_OBJ_MSG("Function has been handled.");
    }
}

INTERNAL_END;
SSS_END;
