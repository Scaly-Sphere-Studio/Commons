#pragma once

#include "_includes.hpp"

/** @file
 *  Color unions and functions.
 */

__SSS_BEGIN;

/** 24 bits RGB color union.
 *  @sa RGBA32.
 */
union RGB24 {    
    uint32_t rgb : 24;  /**< Whole color, containing red, green, and blue.*/
    struct {
        uint8_t r;      /**< Red part of the color.*/
        uint8_t g;      /**< Green part of the color.*/
        uint8_t b;      /**< Blue part of the color.*/
    } bytes;            /**< Holds red, green, and blue parts of the color.*/

    /** Empty constructor, sets color to 0.*/
    RGB24() noexcept : rgb(0) {};
    /** Copy constructor.*/
    RGB24(RGB24 const& color) noexcept : rgb(color.rgb) {};
    /** Constructor, sets the whole color value.*/
    RGB24(uint32_t color) noexcept : rgb(color) {};
    /** Constructor, sets the color values.*/
    RGB24(uint8_t red, uint8_t green, uint8_t blue) noexcept : bytes({red, green, blue}) {};
};

/** 32 bits RGBA color union.
 *  @sa RGB24.
*/
union RGBA32 {
    uint32_t rgba;      /**< Whole color, containing red, green, blue and alpha.*/
    struct {
        uint8_t r;      /**< Red part of the color.*/
        uint8_t g;      /**< Green part of the color.*/
        uint8_t b;      /**< Blue part of the color.*/
        uint8_t a;      /**< Alpha (opacity) part of the color.*/
    } bytes;            /**< Holds red, green, blue and alpha parts of the color.*/

    /** Typedef for pixel arrays.*/
    using Vector = std::vector<RGBA32>;

    /** Empty constructor, sets color to 0.*/
    RGBA32() noexcept : rgba(0) {};
    /** Copy constructor.*/
    RGBA32(RGBA32 const& color32) noexcept : rgba(color32.rgba) {};
    /** Constructor that sets rgb values from RGB24 and sets alpha to given value.*/
    RGBA32(RGB24 const& color24, uint8_t alpha = 255) noexcept
        : rgba(color24.rgb | (alpha << 24)) {};
    /** Constructor, sets the whole color value.*/
    RGBA32(uint32_t color32) noexcept : rgba(color32) {};
    /** Constructor, sets the color values.*/
    RGBA32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) noexcept
        : bytes({red, green, blue, alpha}) {};
};

/** Returns a "rainbow" color based on the ratio of passed values.*/
RGB24 rainbow(uint32_t value, uint32_t max_value) noexcept;

/** Blends colors together based on the source's alpha.*/
RGBA32& operator*=(RGBA32& dst, RGBA32 const& src) noexcept;

__SSS_END;