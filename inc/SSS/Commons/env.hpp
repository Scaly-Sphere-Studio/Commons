#pragma once

#include "SSS/Commons/_includes.hpp"

__SSS_BEGIN

// Copies the corresponding env variable's content in a string.
// An unknown env variable will result in an empty string.
std::string copyEnv(std::string const& varname);

// Returns true if the given path leads to an existing directory.
bool isDir(std::string const& path) noexcept;
// Returns true if the given path leads to an existing regular file.
bool isReg(std::string const& path) noexcept;

__INTERNAL_BEGIN

std::string getPWD();

__INTERNAL_END

// Path to the folder holding the running .exe
static const std::string PWD = _internal::getPWD();

__SSS_END