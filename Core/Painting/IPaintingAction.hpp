#pragma once

#include <QPoint>

#include "PaintingToolType.hpp"

class QPainter;

struct IPaintingAction {

    virtual ~IPaintingAction() {}

    virtual void perform(QPainter*, const QPoint&) = 0;

};
