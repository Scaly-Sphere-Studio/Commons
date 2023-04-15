#ifndef SSS_COMMONS_BASE_HPP
#define SSS_COMMONS_BASE_HPP

#include "_includes.hpp"

SSS_BEGIN;

// Ignore warning about STL exports as they're private members
#pragma warning(push, 2)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

class Base {
public:
    inline Base() {};
    inline ~Base() {};
    inline Base(Base const& src) : name(src.name) {};
    inline Base(Base&& src) noexcept : name(std::move(src.name)) {};
    inline Base& operator=(Base const& src) { name = src.name; return *this; };
    inline Base& operator=(Base&& src) noexcept{ name = std::move(src.name); return *this; };

    inline auto getCreationTime() const noexcept { return creation_time; };
    inline void setName(std::string const& str) noexcept { name = str; };
    inline std::string const& getName() const noexcept { return name; };

private:
    std::chrono::steady_clock::time_point const creation_time { std::chrono::steady_clock::now() };
    std::string name;
};

#pragma warning(pop)

SSS_END;

#endif //SSS_COMMONS_BASE_HPP