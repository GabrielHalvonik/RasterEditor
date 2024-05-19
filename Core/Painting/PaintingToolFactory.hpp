#pragma once

#include "IPaintingAction.hpp"
#include "BrushAction.hpp"
#include "EraserAction.hpp"

struct PaintingToolFactory {

    static IPaintingAction* construct(PaintingToolType type) {
        switch (type) {
            case PaintingToolType::Brush : { return new BrushAction(); } break;
            case PaintingToolType::Eraser : { return new EraserAction(); } break;
            default: return nullptr;
        }
    }

};
