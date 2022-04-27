#pragma once

#include "_includes.hpp"

/** @file
 *  Defines multiple conversion functions.
 */

SSS_BEGIN;

/** Converts given data of type \c T to string if possible.
 *  Given data of type \c T will be fed to an \c std::ostringstream.\n
 *  Hence, type \c T needs to have an <b>insertion operator</b> available.\n
 *  This function isn't the fastest, but aspires to be the most practical.
 *  @param[in] arg The data of type \c T to be converted.
 *  @return A string containing either the converted data or an error
 *  message if an exception was caught <em>(which should not happen)</em>.
 */
template <typename T>
std::string toString(T const& arg) noexcept try
{
    std::ostringstream strstream;
    strstream << std::boolalpha << arg;
    return strstream.str();
}
catch (...) {
    return "[SSS::toString() error]";
};

/** Converts \c std::string to \c std::u32string.
 *  @param[in] str The \c std::string to convert.
 *  @return The converted \c std::u32string.
 *  @sa str32ToStr
 */
std::u32string strToStr32(std::string const& str);
/** Converts \c std::u32string to \c std::string.
 *  As \c std::string supports UTF-8, the resulting
 *  string suffers no loss of data.
 *  @param[in] str The \c std::u32string to convert.
 *  @return The converted \c std::string.
 *  @sa strToStr32
 */
std::string str32ToStr(std::u32string const& str);

SSS_END;