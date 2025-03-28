#pragma once

#include "_includes.hpp"

SSS_BEGIN;

// Ignore warning about STL exports as they're private members
#pragma warning(push, 2)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

class SSS_COMMONS_API Observer;

class SSS_COMMONS_API Subject
{
    friend class Observer;
public:
    Subject() = default;
    Subject(Subject&&) = delete;
    Subject(Subject const&) = delete;
    virtual ~Subject();

    template <class T>
    bool is() const noexcept {
        return dynamic_cast<T const*>(this) != nullptr;
    }

protected:
    void _notifyObservers(int event_id = 0) const;

private:
    std::list<std::reference_wrapper<Observer>> _observers;

    void _attachObserver(Observer& observer);
    void _detachObserver(Observer const& observer);
};

class SSS_COMMONS_API Observer
{
    friend class Subject;
public:
    Observer() = default;
    Observer(Observer&&) = delete;
    Observer(Observer const&) = delete;
    ~Observer();

protected:
    void _ignore(Subject& subject);
    void _observe(Subject& subject);

    template<std::derived_from<Subject> T>
    void _set(std::shared_ptr<T>& dst, std::shared_ptr<T> src) {
        if (dst)
            _ignore(*dst.get());
        if (src)
            _observe(*src.get());
        dst = src;
    }

private:
    std::list<std::reference_wrapper<Subject>> _subjects;

    virtual void _subjectUpdate(Subject const& subject, int event_id) = 0;
};

#pragma warning(pop)

SSS_END;
