#pragma once

#include "PenAction.hpp"
#include "BrushAction.hpp"
#include "EraserAction.hpp"
#include "PaintingToolType.hpp"

struct PaintingToolFactory {

    static std::tuple<PaintingToolType, PaintingActionBase*> construct(PaintingToolType type) {
        switch (type) {
            case PaintingToolType::Brush : { return { type, new BrushAction() }; } break;
            case PaintingToolType::Pen : { return { type, new PenAction() }; } break;
            case PaintingToolType::Eraser : { return { type, new EraserAction() }; } break;
            default: return { PaintingToolType::None, { } };
        }
    }

};
