#pragma once

#include "Bind.hpp"
#include "Slot.hpp"
#include "Event.hpp"
#include "Handler.hpp"
#include "Property.hpp"
#include "Parameter.hpp"
#include "Observable.hpp"

using std::function;
using std::byte;
using std::vector;
using std::string;
using std::tuple;

template <typename T>
using property = Property<T>;

template <typename T>
using list = std::initializer_list<T>;

#define parameters struct Parameters

#define parametrized(owner) \
    owner (Parameters&& Parameters) : owner() { \
        for (int i = 0; i < sizeof(Parameters); ) { \
            i += reinterpret_cast<IBindable*>(reinterpret_cast<std::byte*>(&Parameters) + i)->binds(this); \
        } \
    }

#define parametrize(item) \
    template <typename T> \
    using DeducedParameter = Parameter<T, item>; \
    template <typename T> \
    using DeducedHandler = Handler<T, item>; \
    template <typename T> \
    using DeducedSlot = Slot<T, item>; \
    struct Parameters; \
    parametrized(item) \
    parameters



#define set_parameter using Parameter::Parameter;
#define affect(value) set_parameter target function() { return &value; }
#define parameter struct : DeducedParameter



#define set_slot using Slot::Slot;
#define subscribe(value) set_slot target function() { return &value; }
#define connection struct : Slot



#define set_handler using Handler::Handler;
#define attach(value) set_handler target function() { return &value; }
#define handler struct : DeducedHandler


// #define field Field
#define observable Observable



#define bind(item) Bind { item }
#define into += [this](void* source, auto value) -> std::optional

#define delegate [this](void* source, auto value)


#define setter .setter = [](auto& field, auto value)
#define getter .getter = [](auto& field) -> auto
