#pragma once

#include <vector>
#include <functional>
#include <any>

struct AnonymousObservable {

    using HandlerType = std::function<void(std::any)>;

    AnonymousObservable() { }

    AnonymousObservable& operator = (std::any arg) {
        value = arg;
        asigned = true;

        for (auto& item : handlers) {
            item(arg);
        }
        return *this;
    }

    void asign(std::any arg) {
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

    template <typename S, typename P>
    void bind(S* first, void(S::*second)(P)) {
        source = reinterpret_cast<void*>(first);

        connect([first, second](const std::any& arg) {
            if (arg.has_value()) {
                if (auto val = std::any_cast<std::optional<P>>(arg); val.has_value()) {
                    (first->*second)(val.value());
                }
            }
        });
    }

    template <typename S, typename P>
    void bind(S* first, void(S::*second)(const P&)) {
        source = reinterpret_cast<void*>(first);

        connect([first, second](const std::any& arg) {
            if (arg.has_value()) {
                if (auto val = std::any_cast<std::optional<P>>(arg); val.has_value()) {
                    (first->*second)(val.value());
                }
            }
        });
    }

    inline std::any getValue() const { return value; }
    inline bool getAsigned() const { return asigned; }

    void* source { nullptr };
private:
    std::vector<HandlerType> handlers;
    bool asigned { false };
    std::any value { };
};
