#pragma once

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
extern const std::string PWD;

/** Copies the corresponding environment variable's value in a string.
 *  @return The value of the corresponding variable, or an empty string otherwise.
 */
std::string getEnv(std::string const& varname);

/** Returns true if the given path is accessible.*/
bool pathIsAccessible(std::string const& path) noexcept;
/** Returns true if the given path leads to an accessible directory.*/
bool pathIsDir(std::string const& path) noexcept;
/** Returns true if the given path leads to an accessible regular file.*/
bool pathIsFile(std::string const& path) noexcept;
/** Returns the first accessible option, or the unchanged path:
 *  - Absolute path to regular file
 *  - Relative path to regular file
 *  - Absolute path to directory
 *  - Relative path to directory
 *  Relative paths are built such as `SSS::PWD + path`.
 */
std::string pathWhich(std::string const& path);

/** Copies the contents of an existing file in a string.
 *  @throws std::runtime_error If the file couldn't be read.
 */
std::string readFile(std::string const& filepath);

SSS_END;