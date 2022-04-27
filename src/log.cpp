#include "SSS/Commons/log.hpp"

SSS_BEGIN;

// Logs the given argument to the given stream
static void _log(std::ostream& stream, std::string const& str) noexcept try
{
    static std::mutex mutex;
    std::unique_lock const lock(mutex);

    if (!str.empty()) {
        stream
            << timeUTF() << "|  "   // Print UTF time
            << str                  // Print message
            << std::endl;           // Print line break
    }
}
catch (...) {
}

// Logs the given argument to std::cout
void log_msg(std::string const& str) noexcept
{
    _log(std::cout, str);
}

// Logs the given argument to std::cerr
void log_wrn(std::string const& str) noexcept
{
    _log(std::cerr, CONTEXT_MSG("[WRN]", str));
}

// Logs the given argument to std::cerr
void log_err(std::string const& str) noexcept
{
    _log(std::cerr, CONTEXT_MSG("[ERR]", str));
}

std::string getErrorString(int errnum)
{
    char buf[1024];
    strerror_s(buf, 1024, errnum);
    return std::string(buf);
}

// Throws a runtime_error exception with given arg
void throw_exc(std::string const& str)
{
    throw std::runtime_error(str);
}

SSS_END;