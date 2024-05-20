#pragma once

#include <vector>
#include <unordered_map>
#include <initializer_list>

#include "PaintingActionBase.hpp"
#include "PaintingToolType.hpp"

struct PaintingToolRegistry {

    PaintingToolRegistry() { }

    PaintingToolRegistry(std::initializer_list<std::tuple<PaintingToolType, PaintingActionBase*>>&& actions) {
        registerPaintingActions(std::move(actions));
    }

    ~PaintingToolRegistry() {
        for (auto [key, item] : registry) {
            delete item;
            item = nullptr;
        }
    }

    void registerPaintingActions(std::initializer_list<std::tuple<PaintingToolType, PaintingActionBase*>>&& actions) {
        for (auto& [type, action] : actions) {
            registeredToolTypes.push_back(type);
            registry.emplace(type, action);
        }
    }

    void setCurrentToolType(PaintingToolType toolType) {
        currentTool = registry[toolType];
    }


    PaintingActionBase* getCurrentTool() const {
        return currentTool;
    }

    inline std::vector<PaintingToolType> getRegisteredPaintingToolTypes() const {
        return registeredToolTypes;
    }

private:
    PaintingActionBase* currentTool { };
    std::vector<PaintingToolType> registeredToolTypes { };
    std::unordered_map<PaintingToolType, PaintingActionBase*> registry { };
};
