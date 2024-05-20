#include "BrushAction.hpp"

#include <QPainter>

BrushAction::BrushAction() : brush(new QPixmap(50, 50)) {
    brush->fill(Qt::transparent);
    brush->load(":/resources/brushes/brush.png");
}

BrushAction::~BrushAction() {
    delete brush;
    brush = nullptr;
}

QRect BrushAction::perform(QPainter* painter, const QPoint& point) {
    painter->drawPixmap(point.x() - brush->width() / 2, point.y() - brush->height() / 2, *brush);

    return { point.x() - brush->width() / 2, point.y() - brush->height() / 2, brush->width(), brush->height() };
}
