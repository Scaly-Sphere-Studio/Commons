#pragma once

#include "SSS/Commons/_includes.hpp"

SSS_BEGIN__

template<typename _Ty, typename _Dx, _Dx func>
struct C_Ptr : public std::unique_ptr<_Ty, _Dx> {
    C_Ptr() noexcept : C_Ptr(nullptr) {};
    C_Ptr(_Ty* ptr) noexcept : std::unique_ptr<_Ty, _Dx>(ptr, func) {};
};

SSS_END__