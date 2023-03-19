#ifndef SSS_COMMONS_BASE_HPP
#define SSS_COMMONS_BASE_HPP

#include "_includes.hpp"

SSS_BEGIN;

struct Base {
    std::string name;
    std::chrono::steady_clock::time_point const creation_time { std::chrono::steady_clock::now() };
    Base() = default;
    ~Base() = default;
    inline Base(Base const& src) : name(src.name) {};
    inline Base(Base&& src) noexcept : name(std::move(src.name)) {};
    inline Base& operator=(Base const& src) { name = src.name; return *this; };
    inline Base& operator=(Base&& src) noexcept{ name = std::move(src.name); return *this; };
};

SSS_END;

#endif //SSS_COMMONS_BASE_HPP