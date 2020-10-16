#pragma once

#include "SSS/Commons/_includes.hpp"

__SSS_BEGIN

    // --- Basic functions ---

// Returns a formatted string displaying the current UTF time
std::string UTF_Current_Time();

    // --- FPS_Timer ---

class FPS_Timer {
public:
// --- Functions ---

    // Adds one frame to the counter.
    // Calculates FPS value every second, and return true if the value changed.
    bool addFrame() noexcept;
    // Returns the FPS value
    inline long long get() const noexcept { return _fps; }
    
private:
// --- Variables ---

    long long _frames{ 0 }; // Frames counter
    long long _fps{ 0 };    // FPS value

    // Time the instance was created
    std::chrono::system_clock::time_point const _start_time
        { std::chrono::system_clock::now() };
    // Last time the FPS value was calculated
    std::chrono::system_clock::time_point _stored_time
        { _start_time };
};
__SSS_END