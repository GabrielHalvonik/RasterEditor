#include "EraserAction.hpp"

#include <QPainter>

#include "PaintingToolPreviewWidgets.hpp"

EraserAction::EraserAction() {

    PaintingActionBase::setSize({ 50, 50 });
    PaintingActionBase::setPreviewWidget(new EraserPreviewWidget(PaintingActionBase::getSize()));

}

EraserAction::~EraserAction() {

}

QRect EraserAction::perform(QPainter* painter, const QPoint& point) {
    auto eraserSize = PaintingActionBase::getSize();

    painter->setPen(Qt::GlobalColor::transparent);
    painter->setBrush(QBrush(QColor::fromRgba(0xFFFFFFFF)));
    painter->drawEllipse(point, eraserSize.width() / 2, eraserSize.height() / 2);

    return { point.x() - eraserSize.width() / 2, point.y() - eraserSize.height() / 2, eraserSize.height(), eraserSize.height() };
}
