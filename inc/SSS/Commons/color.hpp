#pragma once

#include "SSS/Commons/_includes.hpp"

__SSS_BEGIN

    // --- RGBA24 ---

// Pre declare this struct which is used in the RGB24 union
__INTERNAL_BEGIN
struct RGB24_s;
__INTERNAL_END

// RGB24 union
union RGB24 {
// --- Aliases ---
    
    // Function ptr
    using f = RGB24(*)(uint32_t);
    // Structure used to pass plain colors or functions
    using s = _internal::RGB24_s;

// --- Constructors ---

    RGB24() noexcept : rgb(0) {};
    RGB24(RGB24 const& src) noexcept
        : rgb(src.rgb) {};
    RGB24(uint32_t value) noexcept
        : rgb(value) {};
    RGB24(uint8_t r, uint8_t g, uint8_t b) noexcept
        : bytes({r, g, b}) {};
    
// --- Variables ---

    uint32_t rgb : 24;  // 0xFFFFFF
    struct {
        uint8_t r;      // 0x0000FF
        uint8_t g;      // 0x00FF00
        uint8_t b;      // 0xFF0000
    } bytes;
};

    // --- RGBA24 structure ---

__INTERNAL_BEGIN
struct RGB24_s {
// --- Constructors ---

    RGB24_s() noexcept {};
    RGB24_s(RGB24 color) noexcept
        : is_plain(true), plain(color) {};
    RGB24_s(RGB24::f const& f) noexcept
        : is_plain(false), func(f) {};
    RGB24_s(RGB24::s const& src) noexcept
        : is_plain(src.is_plain), plain(src.plain), func(src.func) {};

// --- Variables ---

    bool is_plain{ true };  // Whether the color is retrieved via a function or not
    RGB24 plain{ 0 };    // The plain color
    RGB24::f func{ nullptr };   // The function to retrieve a plain color
};
__INTERNAL_END

// Returns a rainbow color based on the passed value.
// Value should be as: 0 <= value <= 1530.
RGB24 rainbow(uint32_t value) noexcept;

    // --- RGBA32 ---

// RGBA32 union
union RGBA32 {
// --- Aliases ---

    // Pixel vector
    using Pixels = std::vector<RGBA32>;
    
// --- Constructors ---

    RGBA32() noexcept : rgba(0) {};
    RGBA32(RGBA32 const& src) noexcept
        : rgba(src.rgba) {};
    RGBA32(RGB24 const& src) noexcept
        : rgba(src.rgb | 0xFF000000) {};
    RGBA32(RGB24 const& src, uint8_t a_) noexcept
        : rgba(src.rgb | (a_ << 24)) {};
    RGBA32(uint32_t value) noexcept
        : rgba(value) {};
    RGBA32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
        : bytes({r, g, b, a}) {};

// --- Variables ---

    uint32_t rgba;  // 0xFFFFFFFF
    struct {
        uint8_t r;  // 0x000000FF
        uint8_t g;  // 0x0000FF00
        uint8_t b;  // 0x00FF0000
        uint8_t a;  // 0xFF000000
    } bytes;
};

// Blends colors together, based on the source's alpha
RGBA32& operator*=(RGBA32& dst, RGBA32 const& src) noexcept;

__SSS_END