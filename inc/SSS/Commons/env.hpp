#ifndef SSS_COMMONS_ENV_HPP
#define SSS_COMMONS_ENV_HPP

#include "_includes.hpp"
#include "log.hpp"

/** @file
 *  Defines environment-related functions and variables.
 */

SSS_BEGIN;

/** States the configuration (Release / Debug).
 *  \c true = Debug, \c false = Release.
 */
extern constexpr bool DEBUGMODE
/** \cond SKIP*/
#ifdef NDEBUG
= false
#else
= true
#endif // NDEBUG
/** \endcond*/
;

/** Path to the folder holding the running .exe*/
SSS_COMMONS_API extern const std::string PWD;

/** Copies the corresponding environment variable's value in a string.
 *  @return The value of the corresponding variable, or an empty string otherwise.
 */
SSS_COMMONS_API std::string getEnv(std::string const& varname);

/** Returns true if the given path is accessible.*/
SSS_COMMONS_API bool pathIsAccessible(std::string const& path) noexcept;
/** Returns true if the given path leads to an accessible directory.*/
SSS_COMMONS_API bool pathIsDir(std::string const& path) noexcept;
/** Returns true if the given path leads to an accessible regular file.*/
SSS_COMMONS_API bool pathIsFile(std::string const& path) noexcept;
/** Returns the first accessible option, or the unchanged path:
 *  - Absolute path to regular file
 *  - Relative path to regular file
 *  - Absolute path to directory
 *  - Relative path to directory
 *  Relative paths are built such as `SSS::PWD + path`.
 */
SSS_COMMONS_API std::string pathWhich(std::string const& path);

/** Copies the contents of an existing file in a string.
 *  @throws std::runtime_error If the file couldn't be read.
 */
SSS_COMMONS_API std::string readFile(std::string const& filepath);

SSS_END;

#endif // SSS_COMMONS_ENV_HPP