#include "Commons/color.hpp"
#include "Commons/log.hpp"

SSS_BEGIN;

RGB24::RGB24(RGBA_f c)
{
    r = std::clamp(static_cast<int>(c._col.r * 255), 0, 255);
    g = std::clamp(static_cast<int>(c._col.g * 255), 0, 255);
    b = std::clamp(static_cast<int>(c._col.b * 255), 0, 255);
}

RGB24::RGB24(const glm::vec3& c)
{
    r = std::clamp(static_cast<int>(c.r * 255), 0, 255);
    g = std::clamp(static_cast<int>(c.g * 255), 0, 255);
    b = std::clamp(static_cast<int>(c.b * 255), 0, 255);
}

RGB24::RGB24(const glm::vec4& c)
{
    r = std::clamp(static_cast<int>(c.r * 255), 0, 255);
    g = std::clamp(static_cast<int>(c.g * 255), 0, 255);
    b = std::clamp(static_cast<int>(c.b * 255), 0, 255);
}

std::string RGB24::to_string() const
{
    char buff[64];
    sprintf_s(buff, "0x%06X (RGB: %d, %d, %d)", rgb, r, g, b);
    return buff;
}

RGB24::operator std::string() const
{
    return to_string();
}

RGB24::operator glm::vec3() const
{
    return glm::vec3(r, g, b)/255.f;
}


RGBA32::RGBA32(const RGBA_f& c)
{
    r = std::clamp(static_cast<int>(c._col.r * 255), 0, 255);
    g = std::clamp(static_cast<int>(c._col.g * 255), 0, 255);
    b = std::clamp(static_cast<int>(c._col.b * 255), 0, 255);
    a = std::clamp(static_cast<int>(c._col.a * 255), 0, 255);
}

RGBA32::RGBA32(const glm::vec3& c)
{
    r = std::clamp(static_cast<int>(c.r * 255), 0, 255);
    g = std::clamp(static_cast<int>(c.g * 255), 0, 255);
    b = std::clamp(static_cast<int>(c.b * 255), 0, 255);
    a = 255;
}

RGBA32::RGBA32(const glm::vec4& c)
{
    r = std::clamp(static_cast<int>(c.r * 255), 0, 255);
    g = std::clamp(static_cast<int>(c.g * 255), 0, 255);
    b = std::clamp(static_cast<int>(c.b * 255), 0, 255);
    a = std::clamp(static_cast<int>(c.a * 255), 0, 255);
}

std::string RGBA32::to_string() const
{
    char buff[64];
    sprintf_s(buff, "0x%08X (RGBA: %d, %d, %d, %d)", rgba, r, g, b, a);
    return buff;
}

RGBA32::operator std::string() const
{
    return to_string();
}

RGBA32::operator glm::vec4() const
{
    return glm::vec4(r, g, b, a) / 255.f;
}

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
    float const a((float)src.a / 255.f),
                a2(1.f - a);
    dst.r = static_cast<uint8_t>(
        (static_cast<float>(src.r) * a) +
        (static_cast<float>(dst.r) * a2)
    );
    dst.g = static_cast<uint8_t>(
        (static_cast<float>(src.g) * a) +
        (static_cast<float>(dst.g) * a2)
    );
    dst.b = static_cast<uint8_t>(
        (static_cast<float>(src.b) * a) +
        (static_cast<float>(dst.b) * a2)
    );
    dst.a = src.a > dst.a ? src.a : dst.a;
    return dst;
}

/* Float colors */
std::string RGBA_f::to_string() const
{
    return std::string{ "(RGBA: "
        + std::format("{:.3f}", _col.r) +
        ", " + std::format("{:.3f}", _col.r) +
        ", " + std::format("{:.3f}", _col.r) +
        ", " + std::format("{:.3f}", _col.r) };
}

RGBA_f::operator std::string() const
{
    return to_string();
}

RGBA_f::operator glm::vec4() const
{
    return _col;
}

RGBA_f::RGBA_f(const std::string& hex) 
{
    *this = RGBA_f::from_Hex(hex);
}

RGBA_f::RGBA_f(const RGB24& col)
{
    _col.r = col.r / 255.0f;
    _col.g = col.g / 255.0f;
    _col.b = col.b / 255.0f;
}

RGBA_f::RGBA_f(const RGBA32& col)
{
    _col.r = col.r / 255.0f;
    _col.g = col.g / 255.0f;
    _col.b = col.b / 255.0f;
    _col.a = col.a / 255.0f;
}

std::string RGBA_f::to_Hex() const
{
    int r, g, b, a;

    r = static_cast<int>(std::floor(_col.r * 255.0f));
    g = static_cast<int>(std::floor(_col.g * 255.0f));
    b = static_cast<int>(std::floor(_col.b * 255.0f));
    a = static_cast<int>(std::floor(_col.a * 255.0f));
    char hexColor[10];

    if (a == 255){ std::snprintf(hexColor, sizeof hexColor, "#%02x%02x%02x", r, g, b); }
    else{ std::snprintf(hexColor, sizeof hexColor, "#%02x%02x%02x%02x", r, g, b, a); }
       
    return SSS::toString(hexColor);
}

glm::vec4 RGBA_f::to_HSL() const
{
    const float cmax = std::max({ _col.r, _col.b, _col.g });
    const float cmin = std::min({ _col.r, _col.b, _col.g });
    const float delta = cmax - cmin;

    float h = 0.0f, s, l;

    //LIGHTNESS
    l = (cmax + cmin) / 2.0f;

    //SATURATION
    s = delta / (1.f - std::abs(2.f * l - 1.f));

    //HUE 
    if (delta == 0.f) {
        return { 0.f , 0.f , l, 1.f };
    }

    if (cmax == _col.r) {
        h = 60.f * std::fmod(((_col.g - _col.b) / delta), 6.f);
        if (h < 0) { h += 360.f; }
    }

    if (cmax == _col.g) {
        h = 60.f * (((_col.b - _col.r) / delta) + 2.f);
    }

    if (cmax == _col.b) {
        h = 60.f * (((_col.r - _col.g) / delta) + 4.f);
    }

    return glm::vec4{ h, s, l, 1.0f };
}



RGBA_f RGBA_f::from_HSL(const glm::vec4& hsl)
{
    RGBA_f res;

    const float C = hsl.y * (1.f - std::abs(2.f * hsl.z - 1.f));
    const float X = C * (1.f - std::abs(std::fmod(hsl.x / 60.0f, 2.0f) - 1.f));
    const float m = hsl.z - C / 2.0f;

    const unsigned int val = static_cast<int>(std::floor(hsl.x / 60.0f));

    switch (val) {
    case 1: res = glm::vec4{ X + m ,C + m, m, hsl.a }; break;
    case 2: res = glm::vec4{ m, C + m, X + m, hsl.a }; break;
    case 3: res = glm::vec4{ m, X + m, C + m, hsl.a }; break;
    case 4: res = glm::vec4{ X + m, m, C + m, hsl.a }; break;
    case 5: res = glm::vec4{ C + m, m, X + m, hsl.a }; break;
    default: res = glm::vec4{ C + m, X + m, m, hsl.a };
    }

    return res;
}

std::vector<std::string> split_str(std::string src, unsigned int split_len) {

    unsigned int nb_split = static_cast<unsigned int>(std::floor(src.size() / split_len));
    std::vector<std::string> split_vec;

    for (unsigned int i = 0; i < nb_split; i++) {
        split_vec.push_back(src.substr(i * split_len, split_len));
    }

    if (src.size() % split_len != 0) {
        split_vec.push_back(src.substr(nb_split * split_len));
    }

    return split_vec;
};

RGBA_f RGBA_f::from_Hex(std::string hex)
{
    //Delete the first # char if there is one
    if (hex.at(0) == '#') {
        hex.erase(0, 1);
    }

    //Check if the hex correspond to a color
    if (hex.size() != 6 && hex.size() != 8) {
        SSS::log_err("Hex value is invalid");
        return glm::vec4{ 1.0f };
    }

    std::vector<std::string> colors = split_str(hex, 2);

    float r = static_cast<float>(std::stoi(colors[0], NULL, 16)) / 255.0f;
    float g = static_cast<float>(std::stoi(colors[1], NULL, 16)) / 255.0f;
    float b = static_cast<float>(std::stoi(colors[2], NULL, 16)) / 255.0f;
    float a = 1.0f;

    if(hex.size() == 8)
        float a = static_cast<float>(std::stoi(colors[3], NULL, 16)) / 255.0f;

    return glm::vec4(r, g, b, a);
}

static float srgb_to_linear(float c) noexcept {
    return c <= 0.04045f ? c / 12.92f : std::pow((c + 0.055f) / 1.055f, 2.4f);
}

static float linear_to_srgb(float c) noexcept {
    return c <= 0.0031308f ? 12.92f * c : 1.055f * std::pow(c, 1.0f / 2.4f) - 0.055f;
}

glm::vec4 RGBA_f::to_Oklab() const
{
    float r = srgb_to_linear(_col.r);
    float g = srgb_to_linear(_col.g);
    float b = srgb_to_linear(_col.b);

    float l = std::cbrt(0.4122214708f * r + 0.5363325363f * g + 0.0514459929f * b);
    float m = std::cbrt(0.2119034982f * r + 0.6806995451f * g + 0.1073969566f * b);
    float s = std::cbrt(0.0883024619f * r + 0.2817188376f * g + 0.6299787005f * b);

    return {
        0.2104542553f * l + 0.7936177850f * m - 0.0040720468f * s,
        1.9779984951f * l - 2.4285922050f * m + 0.4505937099f * s,
        0.0259040371f * l + 0.7827717662f * m - 0.8086757660f * s,
        _col.a
    };
}

RGBA_f RGBA_f::from_Oklab(const glm::vec4& lab)
{
    float l = lab.x + 0.3963377774f * lab.y + 0.2158037573f * lab.z;
    float m = lab.x - 0.1055613458f * lab.y - 0.0638541728f * lab.z;
    float s = lab.x - 0.0894841775f * lab.y - 1.2914855480f * lab.z;

    l = l * l * l;
    m = m * m * m;
    s = s * s * s;

    return glm::vec4{
        linear_to_srgb(std::clamp(+4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s, 0.f, 1.f)),
        linear_to_srgb(std::clamp(-1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s, 0.f, 1.f)),
        linear_to_srgb(std::clamp(-0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s, 0.f, 1.f)),
        lab.w
    };
}

RGBA_f mix(const RGBA_f& a, const RGBA_f& b, float t) noexcept
{
    t = std::clamp(t, 0.f, 1.f);
    return RGBA_f::from_Oklab(glm::mix(a.to_Oklab(), b.to_Oklab(), t));
}

SSS_END;