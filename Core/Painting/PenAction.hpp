#pragma once

#include "IPaintingAction.hpp"

class QPixmap;

struct PenAction : IPaintingAction {

    PenAction();
    ~PenAction();

    void perform(QPainter*, const QPoint&) override;

};
