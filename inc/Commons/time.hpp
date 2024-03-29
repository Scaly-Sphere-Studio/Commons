#ifndef SSS_COMMONS_TIME_HPP
#define SSS_COMMONS_TIME_HPP

#include "_includes.hpp"

/** @file
 *  Defines time-related functions and classes.
 */

SSS_BEGIN;

/** Sleeps precisely until given time point.
 *  This is achieved by regaining CPU control \c 3ms sooner
 *  than given time point, and looping for remaining time.\n
 *  Hence, this function is CPU heavy if used repeatedly
 *  every \c 3ms or less.
 */
SSS_COMMONS_API void sleepUntil(std::chrono::steady_clock::time_point time);

/** Returns duration in a formatted string.
 *  @return Time formated as such : <tt>hh:mm:ss.ms UTF</tt>
 */
template<class _Rep, class _Period>
inline std::string convertTime(std::chrono::duration<_Rep, _Period> const& duration)
{
    using namespace std::chrono;

    // Split time into hours, minutes, seconds and milliseconds
    int const ms(duration_cast<milliseconds>(duration).count() % 1000);
    int const s(duration_cast<seconds>(duration).count() % 60);
    int const m(duration_cast<minutes>(duration).count() % 60);
    int const h(duration_cast<hours>(duration).count() % 24);

    // Format the string via printf
    char buff[64];
    sprintf_s(buff, "%02d:%02d:%02d.%03d", h, m, s, ms);

    return buff;
};

/** Returns current UTF time in a formatted string.
 *  @return Time formated as such : <tt>hh:mm:ss.ms UTF</tt>
 */
SSS_COMMONS_API std::string timeUTF();

// Ignore warning about STL exports as they're private members
#pragma warning(push, 2)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

/** %Stopwatch used to benchmark features time.
 *  Timer starts at creation, is queried with getMS(),
 *  and starts over with reset().
 */
class SSS_COMMONS_API Stopwatch {
private:
    using clock = std::chrono::steady_clock;
    clock::time_point _start{ clock::now() };
public:
    /** Returns milliseconds (integer) since timer started.
     *  @sa getPreciseMS(), getFormatted(), reset().
     */
    long long getMS() const;
    /** Returns milliseconds (floating point) since timer started.
     *  @sa getFormatted(), reset().
     */
    double getPreciseMS() const;
    /** Returns milliseconds since timer started, formatted in a string.
     *  @return Formated duration, eg: "630ms" for a value of 630.
     *  @sa getMS().
     */
    std::string getFormatted() const;
    /** Resets timer.
     *  @sa getMS().
     */
    void reset();
};

/** Counts frames and times longest one each second.*/
class SSS_COMMONS_API FrameTimer {
public:
    /** Adds one frame to the counter, returns \c true every second.
     *  Computes FPS and "longest frame" every second.
     *  @return \c true every time a second has passed, and \c false otherwise.
     *  @sa get(), #longestFrame().
     */
    bool addFrame() noexcept;

    /** Returns current FPS based on current counter and time.
     *  @return Computed FPS.
     *  @sa addFrame(), getFormatted().
     */
    inline long long get() const noexcept { return _fps; }
    /** Returns current FPS formatted in a string.
     *  @return Formatted FPS, eg: "60" for a value of 60.
     *  @sa get().
     */
    inline std::string getFormatted() const { return std::to_string(_fps); };

    /** Returns the longest frame time (in ms) from last recorded second.
     *  @return Time (in ms) of the longest frame from last second.
     * @sa addFrame()
     */
    inline long long longestFrame() const noexcept { return _last_longest_frame; };
    
private:
    long long _frames{ 0 }; // Frames counter
    long long _fps{ 0 };    // FPS value

    long long _current_longest_frame{ 0 };  // Longest frame from current second
    long long _last_longest_frame{ 0 };     // Longest frame from last second

    // Time the instance was created
    std::chrono::system_clock::time_point const _start_time
        { std::chrono::system_clock::now() };
    // Last time the FPS value was calculated
    std::chrono::system_clock::time_point _stored_time
        { _start_time };

    Stopwatch _frame_watch;
};

#pragma warning(pop)

SSS_END;

#endif // SSS_COMMONS_TIME_HPP