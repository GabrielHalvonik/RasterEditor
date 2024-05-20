#include "PenAction.hpp"

PenAction::PenAction() {

}

PenAction::~PenAction() {

}

QRect PenAction::perform(QPainter* painter, const QPoint& point) {
    return { point.x(), point.y(), 0, 0 };
}
