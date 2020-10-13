#pragma once

#include "SSS/Commons/_includes.hpp"

SSS_BEGIN__

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
    long long get() const noexcept;

private:
// --- Variables ---

    long long frames_{ 0 }; // Frames counter
    long long fps_{ 0 };    // FPS value

    // Time the instance was created
    std::chrono::system_clock::time_point const start_time_
        { std::chrono::system_clock::now() };
    // Last time the FPS value was calculated
    std::chrono::system_clock::time_point stored_time_
        { start_time_ };
};
SSS_END__