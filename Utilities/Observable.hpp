#pragma once

#include "AnonymousObservable.hpp"

template <typename T>
struct Observable {

    using HandlerType = std::function<void(T)>;

    Observable() { }

    Observable(T&& arg) : value(std::move(arg)), asigned(true) { }

    ~Observable() {
        for (AnonymousObservable* item : observables) {
            delete item;
            item = nullptr;
        }
    }

    Observable<T>& operator = (T arg) {
        value = arg;
        asigned = true;

        for (auto& item : handlers) {
            item(arg);
        }
        return *this;
    }

    void asign(T arg) {
        value = arg;
        asigned = true;

        for (auto& item : handlers) {
            item(arg);
        }
    }

    void connect(HandlerType&& item) {
        handlers.push_back(std::move(item));
    }

    void operator+=(const HandlerType&& item) {
        handlers.push_back(std::move(item));
    }

    template <typename S>
    void bind(S* first, void(S::*second)(T)) {
        connect([first, second](T arg){ (first->*second)(arg); });
    }

    template <typename S>
    void bind(S* first, void(S::*second)(const T&)) {
        connect([first, second](const T& arg){ (first->*second)(arg); });
    }

    inline T getValue() const { return value; }
    inline bool getAsigned() const { return asigned; }

    void registerAnonymousObservable(AnonymousObservable* observable) {
        observables.push_back(observable);
    }

private:
    std::vector<AnonymousObservable*> observables;
    std::vector<HandlerType> handlers;
    bool asigned { false };
    T value { };
};







template <typename T>
struct Observable<const T&> {

    using HandlerType = std::function<void(T)>;

    Observable() { }

    Observable(T&& arg) : value(std::move(arg)), asigned(true) { }

    ~Observable() {
        for (AnonymousObservable* item : observables) {
            delete item;
            item = nullptr;
        }
    }

    Observable<const T&>& operator = (const T& arg) {
        value = arg;
        asigned = true;

        for (auto& item : handlers) {
            item(arg);
        }
        return *this;
    }

    void asign(T arg) {
        value = arg;
        asigned = true;

        for (auto& item : handlers) {
            item(arg);
        }
    }

    void connect(HandlerType&& item) {
        handlers.push_back(std::move(item));
    }

    void operator+=(const HandlerType&& item) {
        handlers.push_back(std::move(item));
    }

    // template <typename S>
    // void bind(S* first, void(S::*second)(T)) {
    //     connect([first, second](T arg){ (first->*second)(arg); });
    // }

    template <typename S>
    void bind(S* first, void(S::*second)(const T&)) {
        connect([first, second](const T& arg){ (first->*second)(arg); });
    }

    inline T getValue() const { return value; }
    inline bool getAsigned() const { return asigned; }

    void registerAnonymousObservable(AnonymousObservable* observable) {
        observables.push_back(observable);
    }

private:
    std::vector<AnonymousObservable*> observables;
    std::vector<HandlerType> handlers;
    bool asigned { false };
    T value { };
};

