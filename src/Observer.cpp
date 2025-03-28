#include "Commons/Observer.hpp"

SSS_BEGIN;

Subject::~Subject()
{
    for (auto const& observer : _observers) {
        observer.get()._subjects.remove_if([this](auto&& ref) {
            return &ref.get() == this;
        });
    }
}

void Subject::_attachObserver(Observer& observer)
{
    _observers.emplace_back(observer);
}

void Subject::_detachObserver(Observer const& observer)
{
    _observers.remove_if([&observer](auto&& ref) {
        return &ref.get() == &observer;
    });
}

void Subject::_notifyObservers(int event_id) const
{
    for (auto const& ref : _observers) {
        ref.get()._subjectUpdate(*this, event_id);
    }
}

Observer::~Observer()
{
    for (auto const& subject : _subjects) {
        subject.get()._detachObserver(*this);
    }
}

void Observer::_ignore(Subject& subject)
{
    _subjects.remove_if([&subject](auto&& ref) {
        return &ref.get() == &subject;
    });
    subject._detachObserver(*this);
}

void Observer::_observe(Subject& subject)
{
    _subjects.emplace_back(subject);
    subject._attachObserver(*this);
}

SSS_END;