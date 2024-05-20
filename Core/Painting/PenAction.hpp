#pragma once

#include "IPaintingAction.hpp"

class QPixmap;

struct PenAction : IPaintingAction {

    PenAction();
    ~PenAction();

    QRect perform(QPainter*, const QPoint&) override;

};
