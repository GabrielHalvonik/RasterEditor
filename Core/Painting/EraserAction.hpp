#pragma once

#include "IPaintingAction.hpp"

struct EraserAction : IPaintingAction  {

    EraserAction();
    ~EraserAction();

    PaintingToolType toolType() const override { return PaintingToolType::Eraser; }
    void perform(QPainter*, const QPoint&) override;

};
