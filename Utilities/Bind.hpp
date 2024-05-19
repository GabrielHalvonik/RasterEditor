#pragma once

#include "Observable.hpp"

template <typename T>
struct Bind {

    Observable<T>* source;

    Bind(Observable<T>& value) : source(&value) { }

    AnonymousObservable& operator +=(std::function<std::any(void* source, T val)>&& fun) {
        AnonymousObservable* target = new AnonymousObservable();
        source->registerAnonymousObservable(target);

        source->connect([target, fun = std::move(fun)](T value) {
            target->asign(fun(target->source, value));
        });

        return *target;
    }

};
