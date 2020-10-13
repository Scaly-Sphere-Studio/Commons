#include "SSS/Commons/color.hpp"

__SSS_BEGIN

// Blends colors together, based on the source's alpha
BGRA32& operator*=(BGRA32& dst, BGRA32 const& src) noexcept
{
    if (dst.bgra == 0) {
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

// Returns a rainbow color based on the passed value.
// Value should be as: 0 <= value <= 1530.
BGR24 rainbow(uint32_t value) noexcept {
    if (value < 255) {
        return BGR24(255, static_cast<uint8_t>(value), 0);
    }
    else if (value < 510) {
        return BGR24(static_cast<uint8_t>(510 - value), 255, 0);
    }
    else if (value < 765) {
        return BGR24(0, 255, static_cast<uint8_t>(value - 510));
    }
    else if (value < 1020) {
        return BGR24(0, static_cast<uint8_t>(1020 - value), 255);
    }
    else if (value < 1275) {
        return BGR24(static_cast<uint8_t>(value - 1020), 0, 255);
    }
    return BGR24(255, 0, static_cast<uint8_t>(1530 - value));
}

__SSS_END