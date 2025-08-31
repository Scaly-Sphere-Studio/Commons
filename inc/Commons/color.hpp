#ifndef SSS_COMMONS_COLORS_HPP
#define SSS_COMMONS_COLORS_HPP

#include "_includes.hpp"

#pragma warning(disable: 4251)

/** @file
 *  Defines color unions and functions.
 */

SSS_BEGIN;

class RGBA_f;

/** \c 24 bits (Red, Green, Blue) color union.
 *  This format is mostly used for color parameters.
 *  @sa RGBA32
 */
struct SSS_COMMONS_API RGB24 {
    union {
        uint32_t rgb : 24;  /**< \c \b 0xFFFFFF -- Whole color*/
        struct {
            uint8_t r;      /**< \c \b 0x0000FF -- Red part of the color.*/
            uint8_t g;      /**< \c \b 0x00FF00 -- Green part of the color.*/
            uint8_t b;      /**< \c \b 0xFF0000 -- Blue part of the color.*/
        };
    };
    /** Empty constructor, sets color to 0.*/
    RGB24() noexcept : rgb(0) {};
    /** Constructor, sets the whole color value.*/
    RGB24(uint32_t color) noexcept : rgb(color) {};
    /** Constructor, sets the color values.*/
    RGB24(uint8_t red, uint8_t green, uint8_t blue) noexcept : r(red), g(green), b(blue) {};
    /** Constructor, reinterpret values from RGBA_f.*/
    RGB24(RGBA_f c);
    /** Constructor, reinterpret values from vec3.*/
    RGB24(const glm::vec3& c);
    /** Constructor, reinterpret values from vec4.*/
    RGB24(const glm::vec4& c);

    std::string to_string() const;
    operator std::string() const;
    operator glm::vec3() const;

};

/** \c 32 bits (Red, Green, Blue, Alpha) color union.
 *  This format is mostly used for pixel buffers.
 *  @sa RGB24
 */
struct SSS_COMMONS_API RGBA32 {
    union {
        uint32_t rgba;  /**< \c \b 0xFFFFFFFF -- Whole color.*/
        struct {
            uint8_t r;  /**< \c \b 0x000000FF -- Red part of the color.*/
            uint8_t g;  /**< \c \b 0x0000FF00 -- Green part of the color.*/
            uint8_t b;  /**< \c \b 0x00FF0000 -- Blue part of the color.*/
            uint8_t a;  /**< \c \b 0xFF000000 -- Alpha (opacity) part of the color.*/
        };
    };

    /** Typedef for pixel arrays.*/
    using Vector = std::vector<RGBA32>;

    /** Empty constructor, sets color to 0.*/
    RGBA32() noexcept : rgba(0) {};
    /** Constructor that sets rgb values from RGB24, and sets alpha to given value.*/
    RGBA32(RGB24 const& color24, uint8_t alpha = 255) noexcept
        : rgba(color24.rgb | (alpha << 24)) {};
    /** Constructor, sets the whole color value.*/
    RGBA32(uint32_t color32) noexcept : rgba(color32) {};
    /** Constructor, sets the color values.*/
    RGBA32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) noexcept
        : r(red), g(green), b(blue), a(alpha) {};
    /** Constructor that sets rgb values from RGBA_f*/
    RGBA32(const RGBA_f& c);
    /** Constructor that sets rgb values from vec3 values, set alpha to 255*/
    RGBA32(const glm::vec3& c);
    /** Constructor that sets rgb values from vec4 values.*/
    RGBA32(const glm::vec4& c);

    std::string to_string() const;
    operator std::string() const;
    operator glm::vec4() const;
};



/** \c 4 floats (Red, Green, Blue, Alpha) color class.
 *  This format is mostly used for computing colors and conversions
 *  Support hexadecimal and HSL
 *  @sa RGBA_f
 */
class SSS_COMMONS_API RGBA_f 
{
public:
    /** Empty constructor, sets color to 1.0 (white).*/
    RGBA_f() { _col = glm::vec4{ 1.0 }; };
    /** Constructor, sets the color from vec4.*/
    RGBA_f(const glm::vec4& col) 
        :_col(col) {};
    /** Constructor, parse the hexadecimal color code, with or without '#' key.*/
    RGBA_f(const std::string& hex);
    /** Constructor, reinterpret values from RGB24.*/
    RGBA_f(const RGB24& col);
    /** Constructor, reinterpret values from RGB32.*/
    RGBA_f(const RGBA32& col);
    

    std::string to_Hex();
    /** Convert RGBA_f to HSL with alpha value*/
    glm::vec4 to_HSL();
    /** Return the vec4 color*/
    glm::vec4 to_RGBA() const { return _col; } ;
    /** Construct an RGBA_f class from HSLA color space*/
    static RGBA_f from_HSL(const glm::vec4& hsl);
    /** Construct an RGBA_f class from from Hexadecimal color code, can be specified with or without '#' key*/
    static RGBA_f from_Hex(std::string hex);

    std::string to_string() const;
    operator std::string() const;
    operator glm::vec4() const;


    glm::vec4 _col;
};

//Miscellanous

/** Returns a "rainbow" color based on the ratio of passed values.*/
SSS_COMMONS_API RGB24 rainbow(uint32_t value, uint32_t max_value) noexcept;

/** Blends colors together based on the source's alpha.*/
SSS_COMMONS_API RGBA32& operator*=(RGBA32& dst, RGBA32 const& src) noexcept;



SSS_END;

#endif // SSS_COMMONS_LOG_HPP