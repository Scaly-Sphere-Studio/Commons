#include "SSS/Commons/log.hpp"

SSS_BEGIN__

// Logs the given argument to the given stream
static void log_(std::ostream& stream, std::string const& str) noexcept try
{
    if (!str.empty()) {
        stream << UTF_Current_Time() << ' ' << str << std::endl;
    }
}
catch (...) {
}

// Logs the given argument to std::cout
void log_msg(std::string const& str) noexcept
{
    log_(std::cout, str);
}

// Logs the given argument to std::cerr
void log_err(std::string const& str) noexcept
{
    log_(std::cerr, str);
}

// Throws a runtime_error exception with given arg
void throw_exc(std::string const& str)
{
    throw std::runtime_error(str);
}

SSS_END__