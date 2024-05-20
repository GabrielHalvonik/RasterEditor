#pragma once

#include "IPaintingAction.hpp"
#include "BrushAction.hpp"
#include "EraserAction.hpp"
#include "PenAction.hpp"

struct PaintingToolFactory {

    static std::tuple<PaintingToolType, IPaintingAction*> construct(PaintingToolType type) {
        switch (type) {
            case PaintingToolType::Brush : { return { type, new BrushAction() }; } break;
            case PaintingToolType::Pen : { return { type, new PenAction() }; } break;
            case PaintingToolType::Eraser : { return { type, new EraserAction() }; } break;
            default: return { PaintingToolType::None, { } };
        }
    }

};
