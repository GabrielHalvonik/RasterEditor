#pragma once

#include "PaintingActionBase.hpp"

class QPixmap;

struct BrushAction : PaintingActionBase {

    BrushAction();
    ~BrushAction();

    QRect perform(QPainter*, const QPoint&) override;

private:
    QPixmap* brush { };
};

