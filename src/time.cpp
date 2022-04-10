#include "SSS/Commons/time.hpp"
#include "SSS/Commons/log.hpp"

__SSS_BEGIN;

// Returns a formatted string displaying the current UTF time
std::string UTF_Current_Time()
{
    using namespace std::chrono;

    // Retrieve current time
    system_clock::duration const now = system_clock::now().time_since_epoch();

    // Split time into hours, minutes, seconds and milliseconds
    int const ms(duration_cast<milliseconds>(now).count() % 1000);
    int const s(duration_cast<seconds>(now).count() % 60);
    int const m(duration_cast<minutes>(now).count() % 60);
    int const h(duration_cast<hours>(now).count() % 24);

    // Format the string via printf
    char buff[64];
    snprintf(buff, 64U, "%02d:%02d:%02d.%03d UTF", h, m, s, ms);

    return buff;
}

long long Stopwatch::getMS() const
{
    std::chrono::duration<double, std::milli> diff(clock::now() - _start);
    return static_cast<long long>(diff.count());
}

std::string Stopwatch::getFormatted() const
{
    return std::to_string(getMS()) + "ms";
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

__SSS_END;