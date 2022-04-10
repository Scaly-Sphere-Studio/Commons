#pragma once

#include "_includes.hpp"

/** @file
 *  Time related functions and classes.
 */

__SSS_BEGIN;

/** Returns UTF time in a formatted string.
 *  @return Time formated as such : <tt>hh:mm:ss.ms UTF</tt>
 */
std::string UTF_Current_Time();

/** %Stopwatch used to benchmark features time.
 *  Timer starts at creation, can be queried using \c #get(), and
 *  can start over with \c #reset().
 */
class Stopwatch {
private:
    using clock = std::chrono::high_resolution_clock;
    clock::time_point _start{ clock::now() };
public:
    /** Returns milliseconds since timer started.
     *  @sa \c #getFormatted(), \c #reset().
     */
    long long getMS() const;
    /** Returns milliseconds since timer started, formatted in a string.
     *  @return Formated duration, eg: "630ms" for a value of 630.
     *  @sa \c #get().
     */
    std::string getFormatted() const;
    /** Resets timer.
     *  @sa \c #get().
     */
    void reset();
};

/** Counter that computes Frames Per Second (FPS) and longest frame from last second.
 */
class FrameTimer {
public:
    /** Adds one frame to the counter, returns \c true every second.
     *  Computes FPS and "longest frame" every second.
     *  @return \c true every time a second has passed, and \c false otherwise.
     *  @sa \c #get(), #longestFrame().
     */
    bool addFrame() noexcept;

    /** Returns current FPS based on current counter and time.
     *  @return Computed FPS.
     *  @sa \c #addFrame(), \c #getFormatted().
     */
    inline long long get() const noexcept { return _fps; }
    /** Returns current FPS formatted in a string.
     *  @return Formatted FPS, eg: "60" for a value of 60.
     *  @sa \c #get().
     */
    inline std::string getFormatted() const { return std::to_string(_fps); };

    /** Returns the longest frame time (in ms) from last recorded second.
     *  @return Time (in ms) of the longest frame from last second.
     * @sa \c #addFrame()
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

__SSS_END;