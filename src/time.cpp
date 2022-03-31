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

// Adds one frame to the counter.
// Calculates FPS value every second, and return true if the value changed.
bool FPS_Timer::addFrame() noexcept
{
    ++_frames;

    using namespace std::chrono;
    seconds const sec = duration_cast<seconds>(system_clock::now() - _stored_time);

    // Update fps every second
    if (sec >= seconds(1)) {
        // Determine fps
        long long const new_fps = _frames / sec.count();
        // Reset fps and increase stored time
        _frames = 0;
        _stored_time += sec;
        // Update old fps if needed
        if (new_fps != _fps) {
            _fps = new_fps;
            return true;
        }
    }
    return false;
}

long long Stopwatch::get() const
{
    std::chrono::duration<double, std::milli> diff(clock::now() - _start);
    return static_cast<long long>(diff.count());
}

std::string Stopwatch::getFormatted() const
{
    return std::to_string(get()) + "ms";
}

void Stopwatch::reset()
{
    _start = clock::now();
}

__SSS_END;