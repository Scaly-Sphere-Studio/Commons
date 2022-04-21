#pragma once

#include "_includes.hpp"

/** @file
 *  Defines smart pointers related classes and functions.
 */

__SSS_BEGIN;

/** Implementation of \c \b std::unique_ptr for C style pointers.
 * 
 *  This template class allows for C style pointers to be handled safely
 *  inside a \c std::unique_ptr.
 * 
 *  @param[in] _Ty The type to which the pointer points to, eg: \c int
 *  for \c int* pointers.
 *  @param[in] _Dx The type of the related clean-up function (destructor),
 *  eg: <tt> void(*)(void*)</tt> for C's \c free.
 *  @param[in] func The clean-up function, eg: \c free.
 * 
 *  @usage
 *  Let's say you want to hold an \c int pointer created by a third party library, and
 *  freed by \c free. The corresponding instancing type would be:
 *  @code{.cpp}
 *  SSS::C_Ptr<int, void(*)(void*), free>
 *  @endcode
 */
template<typename _Ty, typename _Dx, _Dx func>
class C_Ptr : public std::unique_ptr<_Ty, _Dx> {
public:
    /** Constructor, creates an empty \c std::unique_ptr.
     */
    C_Ptr() noexcept : C_Ptr(nullptr) {};
    /** Constructor, creates a \c std::unique_ptr holding the given pointer.
     *  @param[in] ptr The pointer to be stored inside a \c std::unique_ptr.
     */
    C_Ptr(_Ty* ptr) noexcept : std::unique_ptr<_Ty, _Dx>(ptr, func) {};
};

/** Removes all expired \c std::weak_ptr stored in a \c std::vector.
 *  @param[in,out] vector Reference to the \c std::vector to be cleaned.
 */
template <class T>
void cleanWeakPtrVector(std::vector<std::weak_ptr<T>>& vector) {
    vector.erase(
        std::remove_if(
            vector.begin(),
            vector.end(),
            [](std::weak_ptr<T> weak) { return weak.expired(); }),
        vector.end()
    );
};

__SSS_END;