#pragma once

#include "PaintingActionBase.hpp"

#include <QSize>

struct EraserAction : PaintingActionBase  {

    EraserAction();
    ~EraserAction();

    QRect perform(QPainter*, const QPoint&) override;

private:
    QSize eraserSize { };
};
