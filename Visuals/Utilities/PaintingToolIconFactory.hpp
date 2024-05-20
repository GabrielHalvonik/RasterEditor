#pragma once

#include <QIcon>

#include "../../Core/Painting/PaintingToolType.hpp"

struct PaintingToolIconFactory {

    static const int iconSize = 20;

    static QIcon construct(PaintingToolType type) {
        switch (type) {
            case PaintingToolType::Brush : { return Utilities::Resources::loadSvgIcon(":/resources/icons/brush.svg", { iconSize, iconSize }); } break;
            case PaintingToolType::Pen : { return Utilities::Resources::loadSvgIcon(":/resources/icons/pen.svg", { 20, 20 }); } break;
            case PaintingToolType::Eraser : { return Utilities::Resources::loadSvgIcon(":/resources/icons/eraser.svg", { 20, 20 }); } break;
            default: return { QIcon() };
        }
    }

};
