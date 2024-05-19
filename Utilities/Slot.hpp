#pragma once

#include <QObject>
#include <functional>
#include "Observable.hpp"

#include "Bindable.hpp"

template <typename P, typename T>
struct Slot : IBindable {

    using target = void (T::*)(P);

    Slot() { }
    Slot(Observable<P>& observable) : observable(&observable) { }
    Slot(std::function<void(P)>&& item) : value(std::move(item)), asigned(true) { }
    Slot(std::function<void(void*, P)>&& item) : valueWithSource(std::move(item)) { }

    int binds(void* item) override {
        if (auto source = reinterpret_cast<T*>(item); source != nullptr) {
            if (asigned) {
                QObject::connect(source, function(), value);
            } else if (observable != nullptr) {
                QObject::connect(source, function(), std::bind(&Observable<P>::asign, observable, std::placeholders::_1));
            } else if (valueWithSource.has_value()) {
                QObject::connect(source, function(), std::bind(valueWithSource.value(), source, std::placeholders::_1));
            }
        }
        return sizeof(Slot<P, T>);
    }

protected:
    virtual target function() = 0;

private:
    Observable<P>* observable { nullptr };
    std::function<void(P)> value { };
    std::optional<std::function<void(void*, P)>> valueWithSource { };
    bool asigned { false };
};

template <typename P, typename T>
struct Slot<const P&, T> : IBindable {

    using target = void (T::*)(const P&);

    Slot() { }
    Slot(Observable<P>& observable) : observable(&observable) { }
    Slot(std::function<void(const P&)>&& item) : value(std::move(item)), asigned(true) { }
    Slot(std::function<void(void*, const P&)>&& item) : valueWithSource(std::move(item)) { }

    int binds(void* item) override {
        if (auto source = reinterpret_cast<T*>(item); source != nullptr) {
            if (asigned) {
                QObject::connect(source, function(), value);
            } else if (observable != nullptr) {
                QObject::connect(source, function(), std::bind(&Observable<P>::asign, observable, std::placeholders::_1));
            } else if (valueWithSource.has_value()) {
                QObject::connect(source, function(), std::bind(valueWithSource.value(), source, std::placeholders::_1));
            }
        }
        return sizeof(Slot<const P&, T>);
    }

protected:
    virtual target function() = 0;

private:
    Observable<P>* observable { nullptr };
    std::function<void(const P&)> value { };
    std::optional<std::function<void(void*, const P&)>> valueWithSource { };
    bool asigned { false };
};
