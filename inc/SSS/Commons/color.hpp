#pragma once

#include "SSS/Commons/_includes.hpp"

SSS_BEGIN__

    // --- BGRA24 ---

// Pre declare this struct which is used in the BGR24 union
INTERNAL_BEGIN__
struct BGR24_s;
INTERNAL_END__

// BGR24 union
union BGR24 {
// --- Aliases ---
    
    // Function ptr
    using f = BGR24(*)(uint32_t);
    // Structure used to pass plain colors or functions
    using s = _internal::BGR24_s;

// --- Constructors ---

    BGR24() noexcept : bgr(0) {};
    BGR24(BGR24 const& src) noexcept
        : bgr(src.bgr) {};
    BGR24(uint32_t value) noexcept
        : bgr(value) {};
    BGR24(uint8_t r, uint8_t g, uint8_t b) noexcept
        : bytes({b, g, r}) {};
    
// --- Variables ---

    uint32_t bgr : 24;  // 0xFFFFFF
    struct {
        uint8_t b;      // 0x0000FF
        uint8_t g;      // 0x00FF00
        uint8_t r;      // 0xFF0000
    } bytes;
};

    // --- BGRA24 structure ---

INTERNAL_BEGIN__
struct BGR24_s {
// --- Constructors ---

    BGR24_s() noexcept {};
    BGR24_s(BGR24 color) noexcept
        : is_plain(true), plain(color) {};
    BGR24_s(BGR24::f const& f) noexcept
        : is_plain(false), func(f) {};
    BGR24_s(BGR24::s const& src) noexcept
        : is_plain(src.is_plain), plain(src.plain), func(src.func) {};

// --- Variables ---

    bool is_plain{ true };  // Whether the color is retrieved via a function or not
    BGR24 plain{ 0 };    // The plain color
    BGR24::f func{ nullptr };   // The function to retrieve a plain color
};
INTERNAL_END__

// Returns a rainbow color based on the passed value.
// Value should be as: 0 <= value <= 1530.
BGR24 rainbow(uint32_t value) noexcept;

    // --- BGRA32 ---

// BGRA32 union
union BGRA32 {
// --- Aliases ---

    // Pixel vector
    using Pixels = std::vector<BGRA32>;
    
// --- Constructors ---

    BGRA32() noexcept : bgra(0) {};
    BGRA32(BGRA32 const& src) noexcept
        : bgra(src.bgra) {};
    BGRA32(BGR24 const& src) noexcept
        : bgra(src.bgr | 0xFF000000) {};
    BGRA32(BGR24 const& src, uint8_t a_) noexcept
        : bgra(src.bgr | (a_ << 24)) {};
    BGRA32(uint32_t value) noexcept
        : bgra(value) {};
    BGRA32(uint8_t a, uint8_t r, uint8_t g, uint8_t b) noexcept
        : bytes({b, g, r, a}) {};

// --- Variables ---

    uint32_t bgra;  // 0xFFFFFFFF
    struct {
        uint8_t b;  // 0x000000FF
        uint8_t g;  // 0x0000FF00
        uint8_t r;  // 0x00FF0000
        uint8_t a;  // 0xFF000000
    } bytes;
};

// Blends colors together, based on the source's alpha
BGRA32& operator*=(BGRA32& dst, BGRA32 const& src) noexcept;

SSS_END__