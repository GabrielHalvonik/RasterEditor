#pragma once

#include "PaintingActionBase.hpp"

class QPixmap;

struct PenAction : PaintingActionBase {

    PenAction();
    ~PenAction();

    QRect perform(QPainter*, const QPoint&) override;

};
