#include "EraserAction.hpp"

#include <QPainter>

#include "PaintingToolPreviewWidgets.hpp"

EraserAction::EraserAction() {
    eraserSize = { 50, 50 };

    PaintingActionBase::setPreviewWidget(new EraserPreviewWidget(eraserSize));
}

EraserAction::~EraserAction() {

}

QRect EraserAction::perform(QPainter* painter, const QPoint& point) {
    painter->setPen(Qt::GlobalColor::transparent);
    painter->setBrush(QBrush(QColor::fromRgba(0xFFFFFFFF)));
    painter->drawEllipse(point, eraserSize.width() / 2, eraserSize.height() / 2);

    return { point.x() - eraserSize.width(), point.y() - eraserSize.height(), eraserSize.height(), eraserSize.height() };
}
