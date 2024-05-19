#pragma once

#include "Event.hpp"
#include "Bindable.hpp"
#include "Observable.hpp"

template <typename P, typename T>
struct Handler : IBindable {

    using target = Event<P> T::*;

    Handler() { }

    Handler(Observable<P>& observable) : observable(&observable) { }
    Handler(std::function<void(P)>&& item) : value(std::move(item)), asigned(true) { }
    Handler(std::function<void(void*, P)>&& item) : valueWithSource(std::move(item)) { }

    int binds(void* item) override {
        if (auto source = reinterpret_cast<T*>(item); source != nullptr) {
            if (asigned) {
                source->*function() += value;
            } else if (observable != nullptr) {
                source->*function() += std::bind(&Observable<P>::asign, observable, std::placeholders::_1);
            } else if (valueWithSource.has_value()) {
                source->*function() += std::bind(valueWithSource.value(), source, std::placeholders::_1);
            }
        }

        return sizeof(Handler<P, T>);
    }

protected:
    virtual target function() = 0;

private:
    Observable<P>* observable { nullptr };
    std::function<void(P)> value { };
    std::optional<std::function<void(void*, P)>> valueWithSource { };
    bool asigned { false };
};
