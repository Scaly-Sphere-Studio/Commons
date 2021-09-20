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

std::string readFile(std::string const& filepath)
{
    // Read the shader code from the file
    std::string ret;
    std::ifstream stream(filepath, std::ios::in);
    // Throw if file could not be open
    if (!stream.is_open()) {
        throw_exc("Could not open file \'" + filepath + "\' : " + getErrorString(errno));
    }
    std::stringstream sstr;
    sstr << stream.rdbuf();
    ret = sstr.str();
    stream.close();
    return ret;
}

__INTERNAL_BEGIN

// Includes for getPWD();
#if defined(_WIN32)
# include <windows.h>
# include <libloaderapi.h>
#elif defined(_APPLE_) && defined(_MACH_)
#elif defined(linux) || defined(__linux)
#endif

std::string getPWD()
{
    std::string pwd;
#if defined(_WIN32)
    CHAR path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    pwd = std::string(path);
    pwd = pwd.substr(0, pwd.find_last_of("\\/") + 1);
#elif defined(_APPLE_) && defined(_MACH_)
    __LOG_WRN("SSS::PWD can not been set yet on this OS."));
#elif defined(linux) || defined(__linux)
    __LOG_WRN("SSS::PWD can not been set yet on this OS."));
#endif
    return pwd;
}

__INTERNAL_END

__SSS_END