#include "SSS/Commons/conversions.hpp"
#include <codecvt>

__SSS_BEGIN;

#pragma warning(push)
#pragma warning(disable:4996)

std::u32string strToStr32(std::string const& str)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cnv;
    std::u32string utf8 = cnv.from_bytes(str);
    if (cnv.converted() < str.size())
        throw std::runtime_error("incomplete conversion");
    return utf8;
}

std::string str32ToStr(std::u32string const& str)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cnv;
    std::string utf8 = cnv.to_bytes(str);
    if (cnv.converted() < str.size())
        throw std::runtime_error("incomplete conversion");
    return utf8;
}

#pragma warning(pop)

__SSS_END;