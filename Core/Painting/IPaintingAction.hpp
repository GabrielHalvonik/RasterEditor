#pragma once

#include <QRect>
#include <QPoint>

class QPainter;

struct IPaintingAction {

    virtual ~IPaintingAction() {}

    virtual QRect perform(QPainter*, const QPoint&) = 0;

};
