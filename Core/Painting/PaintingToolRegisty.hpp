#pragma once

#include <vector>
#include <unordered_map>
#include <initializer_list>

#include "IPaintingAction.hpp"
#include "PaintingToolType.hpp"

struct PaintingToolRegistry {

    PaintingToolRegistry() { }

    PaintingToolRegistry(std::initializer_list<std::tuple<PaintingToolType, IPaintingAction*>>&& actions) {
        registerPaintingActions(std::move(actions));
    }

    ~PaintingToolRegistry() {
        for (auto [key, item] : registry) {
            delete item;
            item = nullptr;
        }
    }

    void registerPaintingActions(std::initializer_list<std::tuple<PaintingToolType, IPaintingAction*>>&& actions) {
        for (auto& [type, action] : actions) {
            registeredToolTypes.push_back(type);
            registry.emplace(type, action);
        }
    }

    void setCurrentToolType(PaintingToolType toolType) {
        currentTool = registry[toolType];
    }


    IPaintingAction* getCurrentTool() const {
        return currentTool;
    }

    inline std::vector<PaintingToolType> getRegisteredPaintingToolTypes() const {
        return registeredToolTypes;
    }

private:
    IPaintingAction* currentTool { };
    std::vector<PaintingToolType> registeredToolTypes { };
    std::map<PaintingToolType, IPaintingAction*> registry { };
};
