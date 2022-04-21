#pragma once

#include "_includes.hpp"
#include "log.hpp"

/** @file
 *  Defines environment-related functions and variables.
 */

__SSS_BEGIN;

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

/** Copies the contents of an existing file in a string.
 *  @throws std::runtime_error If the file couldn't be read.
 */
std::string readFile(std::string const& filepath);

__SSS_END;