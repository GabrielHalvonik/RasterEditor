#pragma once

#include "Bindable.hpp"
#include "Observable.hpp"

#include <QDebug>

template <typename P, typename T>
struct Parameter : IBindable {

    using target = void (T::*)(P);

    Parameter() { }
    Parameter(P item) : value(item), asigned(true) { }
    Parameter(Observable<P>* item) : observable(item) { value = observable->getValue(); asigned = observable->getAsigned(); }
    Parameter(Observable<P>& item) : observable(&item) { value = observable->getValue(); asigned = observable->getAsigned(); }

    Parameter(AnonymousObservable& item) : Parameter(&item) { }
    Parameter(AnonymousObservable* item) : bindedObservable(item) {
        if (bindedObservable->getAsigned()) {
            asigned = true;
            value = std::any_cast<P>(bindedObservable->getValue());
        }
    }

    int binds(void* item) override {
        if (T* source = reinterpret_cast<T*>(item); source != nullptr) {
            if (asigned) {
                (source->*function())(value);
            }
            if (observable != nullptr) {
                observable->bind(source, function());
            }
            if (bindedObservable != nullptr) {
                bindedObservable->bind(source, function());
            }
        }
        return sizeof(Parameter<P, T>);
    }

protected:
    virtual target function() = 0;

private:
    P value { };
    bool asigned { false };
    Observable<P>* observable { nullptr };
    AnonymousObservable* bindedObservable { nullptr };
};

template <typename P, typename T>
struct Parameter<const P&, T> : IBindable {

    using target = void (T::*)(const P&);

    Parameter() { }
    Parameter(const P& item) : value(std::move(item)), asigned(true) { }
    Parameter(Observable<P>* item) : observable(item) { value = observable->getValue(); asigned = observable->getAsigned(); }
    Parameter(Observable<P>& item) : observable(&item) { value = observable->getValue(); asigned = observable->getAsigned(); }

    Parameter(AnonymousObservable& item) : Parameter(&item) { }
    Parameter(AnonymousObservable* item) : bindedObservable(item) {
        if (bindedObservable->getAsigned()) {
            asigned = true;
            value = std::any_cast<P>(bindedObservable->getValue());
        }
    }

    int binds(void* item) override {
        if (T* source = reinterpret_cast<T*>(item); source != nullptr) {
            if (asigned) {
                (source->*function())(value);
            }
            if (observable != nullptr) {
                observable->bind(source, function());
            }
            if (bindedObservable != nullptr) {
                bindedObservable->bind(source, function());
            }
        }
        return sizeof(Parameter<const P&, T>);
    }

protected:
    virtual target function() = 0;

private:
    P value { };
    bool asigned { false };
    Observable<P>* observable { nullptr };
    AnonymousObservable* bindedObservable { nullptr };
};
