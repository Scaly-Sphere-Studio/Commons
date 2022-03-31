#pragma once

#include "_includes.hpp"
#include "log.hpp"

/** @file
 *  Environment interaction features.
 */

__SSS_BEGIN;

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

/** Copies the contents of an existing file in a string.
 *  @throws std::runtime_error If the file couldn't be read.
 */
std::string readFile(std::string const& filepath);

/** \cond INTERNAL */
__INTERNAL_BEGIN;

std::string getPWD(); // Inits SSS::PWD

__INTERNAL_END;
/** \endcond */

/** Path to the folder holding the running .exe */
static const std::string PWD = _internal::getPWD();

__SSS_END;