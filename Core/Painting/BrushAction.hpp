#pragma once

#include "IPaintingAction.hpp"

class QPixmap;

struct BrushAction : IPaintingAction {

    BrushAction();
    ~BrushAction();

    void perform(QPainter*, const QPoint&) override;

private:
    QPixmap* brush { };
};

