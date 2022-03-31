#include "SSS/Commons/color.hpp"

__SSS_BEGIN;

// Returns a rainbow color based on the passed value.
RGB24 rainbow(uint32_t value, uint32_t max_value) noexcept
{
    // Value should be as: 0 <= value <= 1530.
    uint32_t const index = static_cast<uint32_t>(1530.f *
        (static_cast<float>(value) / static_cast<float>(max_value)));
    if (index < 255) {
        return RGB24(255, static_cast<uint8_t>(index), 0);
    }
    else if (index < 510) {
        return RGB24(static_cast<uint8_t>(510 - index), 255, 0);
    }
    else if (index < 765) {
        return RGB24(0, 255, static_cast<uint8_t>(index - 510));
    }
    else if (index < 1020) {
        return RGB24(0, static_cast<uint8_t>(1020 - index), 255);
    }
    else if (index < 1275) {
        return RGB24(static_cast<uint8_t>(index - 1020), 0, 255);
    }
    return RGB24(255, 0, static_cast<uint8_t>(1530 - index));
}

// Blends colors together, based on the source's alpha
RGBA32& operator*=(RGBA32& dst, RGBA32 const& src) noexcept
{
    if (dst.rgba == 0) {
        return dst = src;
    }
    float const a((float)src.bytes.a / 255.f),
                a2(1.f - a);
    dst.bytes.r = static_cast<uint8_t>(
        (static_cast<float>(src.bytes.r) * a) +
        (static_cast<float>(dst.bytes.r) * a2)
    );
    dst.bytes.g = static_cast<uint8_t>(
        (static_cast<float>(src.bytes.g) * a) +
        (static_cast<float>(dst.bytes.g) * a2)
    );
    dst.bytes.b = static_cast<uint8_t>(
        (static_cast<float>(src.bytes.b) * a) +
        (static_cast<float>(dst.bytes.b) * a2)
    );
    dst.bytes.a = src.bytes.a > dst.bytes.a ? src.bytes.a : dst.bytes.a;
    return dst;
}

__SSS_END;