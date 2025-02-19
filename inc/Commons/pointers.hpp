#ifndef SSS_COMMONS_POINTERS_HPP
#define SSS_COMMONS_POINTERS_HPP

#include "_includes.hpp"
#include "Base.hpp"

/** @file
 *  Defines smart pointers related classes and functions.
 */

SSS_BEGIN;

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
inline void cleanWeakPtrVector(std::vector<std::weak_ptr<T>>& vector) {
    vector.erase(
        std::remove_if(
            vector.begin(),
            vector.end(),
            [](std::weak_ptr<T> weak) { return weak.expired(); }),
        vector.end()
    );
};

template<class T>
class SharedClass : public Base, public std::enable_shared_from_this<T> {
public:
    using Shared = std::shared_ptr<T>;
    using Weak = std::weak_ptr<T>;

    static Shared create()
    {
        return Shared(new T());
    }
};

template<class T>
class InstancedClass : public SharedClass<T> {
private:
    using WeakVector = std::vector<SharedClass<T>::Weak>;
    static WeakVector _instances;
public:
    using Vector = std::vector<SharedClass<T>::Shared>;

    ~InstancedClass() {
        cleanWeakPtrVector(_instances);
    };

    static SharedClass<T>::Shared create() {
        auto ret = SharedClass<T>::create();
        _instances.emplace_back(ret);
        return ret;
    }

    static Vector getInstances() {
        Vector ret;
        ret.reserve(_instances.size());
        for (auto const& ptr : _instances) {
            ret.emplace_back(ptr.lock());
        }
        return ret;
    }

    static SharedClass<T>::Shared get(T const* ptr) {
        for (auto const& weak : _instances) {
            if (auto shared = weak.lock(); shared.get() == ptr)
                return shared;
        }
        return nullptr;
    }
};

template<class T>
InstancedClass<T>::WeakVector InstancedClass<T>::_instances;

SSS_END;

#endif // SSS_COMMONS_POINTERS_HPP