#pragma once

#include "IPaintingAction.hpp"

class QPixmap;

struct BrushAction : IPaintingAction {

    BrushAction();
    ~BrushAction();

    PaintingToolType toolType() const override { return PaintingToolType::Brush; }
    void perform(QPainter*, const QPoint&) override;

private:
    QPixmap* brush { };
};

