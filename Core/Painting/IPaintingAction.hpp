#pragma once

#include <QPoint>

class QPainter;

struct IPaintingAction {

    virtual ~IPaintingAction() {}

    virtual void perform(QPainter*, const QPoint&) = 0;

};
