#pragma once

#include <map>
#include <initializer_list>

#include "IPaintingAction.hpp"

struct PaintingToolRegistry {

    PaintingToolRegistry() { }

    PaintingToolRegistry(std::initializer_list<IPaintingAction*>&& actions) {
        registerPaintingActions(std::move(actions));
    }

    void registerPaintingActions(std::initializer_list<IPaintingAction*>&& actions) {
        for (IPaintingAction* action : actions) {
            registry.emplace(action->toolType(), action);
        }
    }

    void setCurrentToolType(PaintingToolType toolType) {
        currentTool = registry[toolType];
    }

    IPaintingAction* getCurrentTool() const {
        return currentTool;
    }

    ~PaintingToolRegistry() {
        for (auto [key, item] : registry) {
            delete item;
            item = nullptr;
        }
    }

private:
    IPaintingAction* currentTool { };
    std::map<PaintingToolType, IPaintingAction*> registry { };
};
