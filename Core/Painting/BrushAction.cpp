#include "BrushAction.hpp"

#include <QPainter>

BrushAction::BrushAction() : brush(new QPixmap(50, 50)) {
    // brush->pix
}

BrushAction::~BrushAction() {
    delete brush;
    brush = nullptr;
}

void BrushAction::perform(QPainter* painter, const QPoint& point) {
    // painter->drawPixmap(brush);
}
