#include "PenAction.hpp"

#include <QPainter>

PenAction::PenAction() {

}

PenAction::~PenAction() {

}

QRect PenAction::perform(QPainter* painter, const QPoint& point) {
    painter->drawPoint(point.x(), point.y());
    return { point.x(), point.y(), 1, 1 };
}
