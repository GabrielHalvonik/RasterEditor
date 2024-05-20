#pragma once

#include "IPaintingAction.hpp"

#include <QSize>

struct EraserAction : IPaintingAction  {

    EraserAction();
    ~EraserAction();

    QRect perform(QPainter*, const QPoint&) override;

private:
    QSize eraserRadius { };
};
