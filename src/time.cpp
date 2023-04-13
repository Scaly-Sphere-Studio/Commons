#include "SSS/Commons/time.hpp"
#include "SSS/Commons/log.hpp"
#include <Windows.h>
#include <timeapi.h>

SSS_BEGIN;

using namespace std::chrono;

void sleepUntil(steady_clock::time_point time)
{
    static constexpr milliseconds limit(3);
    if (time - steady_clock::now() > limit) {
        timeBeginPeriod(1);
        std::this_thread::sleep_until(time - limit);
        timeEndPeriod(1);
    }
    while (steady_clock::now() < time); // CPU heavy
}

// Returns a formatted string displaying the current UTF time
std::string timeUTF()
{
    return convertTime(system_clock::now().time_since_epoch());
}

long long Stopwatch::getMS() const
{
    duration<double, std::milli> diff(clock::now() - _start);
    return static_cast<long long>(diff.count());
}

double Stopwatch::getPreciseMS() const
{
    duration<double, std::milli> diff(clock::now() - _start);
    return diff.count();
}

std::string Stopwatch::getFormatted() const
{
    char buff[64];
    sprintf_s(buff, "%.2fms", getPreciseMS());
    return buff;
}

void Stopwatch::reset()
{
    _start = clock::now();
}

// Adds one frame to the counter.
// Calculates FPS value every second, and return true if the value changed.
bool FrameTimer::addFrame() noexcept
{
    using namespace std::chrono;

    // Add one frame
    ++_frames;

    // Update current longest frame
    long long const frame_ms = _frame_watch.getMS();
    _frame_watch.reset();
    if (frame_ms > _current_longest_frame) {
        _current_longest_frame = frame_ms;
    }

    // Count seconds since last update
    seconds const sec = duration_cast<seconds>(system_clock::now() - _stored_time);

    // Update values every second
    if (sec >= seconds(1)) {
        // Update longest frame
        _last_longest_frame = _current_longest_frame;
        _current_longest_frame = 0;
        // Determine fps
        long long const new_fps = _frames / sec.count();
        // Reset fps and increase stored time
        _frames = 0;
        _stored_time += sec;
        // Update old fps if needed
        if (new_fps != _fps) {
            _fps = new_fps;
        }
        return true;
    }
    return false;
}

SSS_END;