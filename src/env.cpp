#include "Commons/env.hpp"

// Includes for _getPWD();
#if defined(_WIN32)
# include <windows.h>
# include <libloaderapi.h>
#elif defined(_APPLE_) && defined(_MACH_)
#elif defined(linux) || defined(linux)
#endif

static std::string _getPWD()
{
    std::string pwd;
#if defined(_WIN32)
    CHAR path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    pwd = std::string(path);
    pwd = pwd.substr(0, pwd.find_last_of("\\/") + 1);
#elif defined(_APPLE_) && defined(_MACH_)
    LOG_WRN("SSS::PWD can not been set yet on this OS."));
#elif defined(linux) || defined(linux)
    LOG_WRN("SSS::PWD can not been set yet on this OS."));
#endif
    return pwd;
}

SSS_COMMONS_API std::string const SSS::PWD{ _getPWD() };

SSS_BEGIN;

std::string getEnv(std::string const& varname)
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

bool pathIsAccessible(std::string const& path) noexcept
{
    struct stat s;
    return stat(path.c_str(), &s) == 0;
}

bool pathIsDir(std::string const& path) noexcept
{
    struct stat s;
    return stat(path.c_str(), &s) == 0 && (s.st_mode & S_IFDIR) != 0;
}

bool pathIsFile(std::string const& path) noexcept
{
    struct stat s;
    return stat(path.c_str(), &s) == 0 && (s.st_mode & S_IFREG) != 0;
}

std::string pathWhich(std::string const& path)
{
    std::string const& abs = path;      // Absolute path
    std::string const rel = PWD + path; // Relative path

    // Look for absolute path to regular file
    if (SSS::pathIsFile(abs)) {
        return abs;
    }
    // Look for relative path to regular file
    else if (SSS::pathIsFile(rel)) {
        return rel;
    }
    // Look for absolute path to directory
    if (SSS::pathIsDir(abs)) {
        return abs;
    }
    // Look for relative path to directory
    else if (SSS::pathIsDir(rel)) {
        return rel;
    }
    // Nothing found, return unchanged path
    return path;
}

std::string readFile(std::string const& filepath) try
{
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
CATCH_AND_RETHROW_FUNC_EXC;

SSS_END;