#include "EraserAction.hpp"

#include <QPainter>

EraserAction::EraserAction() {
    eraserRadius = { 25, 25 };
}

EraserAction::~EraserAction() {

}

QRect EraserAction::perform(QPainter* painter, const QPoint& point) {
    painter->setPen(Qt::GlobalColor::transparent);
    painter->setBrush(QBrush(QColor::fromRgba(0xFFFFFFFF)));
    painter->drawEllipse(point, eraserRadius.width(), eraserRadius.height());

    return { point.x() - eraserRadius.width(), point.y() - eraserRadius.height(), eraserRadius.height() * 2, eraserRadius.height() * 2 };
}
