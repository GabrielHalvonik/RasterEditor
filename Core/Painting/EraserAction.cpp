#include "EraserAction.hpp"

#include <QPainter>

EraserAction::EraserAction() {
    eraserSize = { 25, 25 };
}

EraserAction::~EraserAction() {

}

void EraserAction::perform(QPainter* painter, const QPoint& point) {
    painter->setPen(QColor::fromRgba(0xFFFFFFFF));
    painter->setBrush(QBrush(QColor::fromRgba(0xFFFFFFFF)));
    painter->drawEllipse(point, eraserSize.width(), eraserSize.height());
}
