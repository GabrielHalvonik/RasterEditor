#pragma once

#include <functional>

template <typename ...Args>
struct Event {

    using HandlerType = std::function<void(Args...)>;

    void connect(HandlerType&& item) {
        handlers.push_back(std::move(item));
    }

    void operator += (const HandlerType& item) {
        handlers.push_back(item);
    }

    void operator += (HandlerType&& item) {
        handlers.push_back(std::move(item));
    }

    template <typename S>
    void bind(S* first, void(S::*second)(Args...)) {
        connect([first, second](Args... args){ (first->*second)(args...); });
    }

    void raise(const Args&... args) const {
        for (auto& item : handlers) {
            item(args...);
        }
    }

    void operator()(const Args&... args) const {
        for (auto& item : handlers) {
            item(args...);
        }
    }

private:
    std::vector<HandlerType> handlers;
};

