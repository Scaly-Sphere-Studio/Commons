#include "SSS/Commons/env.hpp"

__SSS_BEGIN

// Copies the corresponding env variable's content in a string.
// An unknown env variable will result in an empty string.
std::string copyEnv(std::string const& varname)
{
    std::string ret;
    // Retrieve the var's content
    char* buffer;
    _dupenv_s(&buffer, NULL, varname.c_str());
    // Copy the var's content
    if (buffer != nullptr) {
        ret = buffer;
        // Free the retrieved content
        free(buffer);
    }
    return ret;
}

// Returns true if the given path leads to an existing directory.
bool isDir(std::string const& path) noexcept
{
    struct stat s;
    return stat(path.c_str(), &s) == 0 && (s.st_mode & S_IFDIR) != 0;
}

// Returns true if the given path leads to an existing regular file.
bool isReg(std::string const& path) noexcept
{
    struct stat s;
    return stat(path.c_str(), &s) == 0 && (s.st_mode & S_IFREG) != 0;
}
__SSS_END