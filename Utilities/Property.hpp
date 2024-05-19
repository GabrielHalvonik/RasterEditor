#pragma once

#include <utility>

template <typename T>
struct FieldBase {
protected:
    T field;
};

template <typename T>
struct Property : FieldBase<T> {

    std::optional<void(*)(T& field, T value)> setter { };
    std::optional<T(*)(T& field)> getter { };

    inline void set(T value) {
        qInfo() << "Property::set";
        if (setter.has_value()) {
            setter.value()(this->field, value);
        } else {
            this->field = value;
        }
    }

    inline T get() {
        qInfo() << "Property::get";
        if (getter.has_value()) {
            return getter.value()(this->field);
        } else {
            return this->field;
        }
    }

    Property<T>& operator = (Property<T>& second) {
        set(second.get());
        return *this;
    }

    Property<T>& operator = (T value) {
        set(value);
        return *this;
    }

    operator T () { return get(); }

};
