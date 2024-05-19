#pragma once

#include <QPoint>

#include "PaintingToolType.hpp"

class QPainter;

struct IPaintingAction {

    virtual ~IPaintingAction() {}

    virtual PaintingToolType toolType() const = 0;
    virtual void perform(QPainter*, const QPoint&) = 0;

};
