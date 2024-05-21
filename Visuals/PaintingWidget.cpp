#include "PaintingWidget.hpp"

#include <QColor>
#include <QTime>
#include <QDebug>
#include <QPainter>
#include <QFileDialog>
#include <QPaintEvent>

#include "../Core/Utilities/General.hpp"
#include "../Core/Painting/PaintingToolRegisty.hpp"

PaintingWidget::PaintingWidget(PaintingToolRegistry* registry, const QSize& pageSize, QWidget* parent) :
    QOpenGLWidget(parent),
    paintingToolRegistry(registry),
    pageSize(pageSize)
{
    QOpenGLWidget::setFixedSize(pageSize);

    image.fill(Qt::white);
}

PaintingWidget::~PaintingWidget() { }

void PaintingWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void PaintingWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void PaintingWidget::paintGL() {
    QPainter painter(this);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    painter.drawImage(0, 0, image);
}

void PaintingWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (paintingToolRegistry->getCurrentTool() == nullptr) return;

        isDrawing = true;
        affectedRegion = QRect(event->pos(), event->pos());

        bitmapSnapshot = getImageSnapshot(image.bits(), image.width(), image.rect());
        applyPaintingAction(event->pos());
        previousPoint = event->pos();
        update();
    }
}

void PaintingWidget::mouseMoveEvent(QMouseEvent* event) {
    if (paintingToolRegistry->getCurrentTool() == nullptr) return;

    if (paintingToolPreviewWidget = paintingToolRegistry->getCurrentTool()->getPreviewWidget(); paintingToolPreviewWidget != nullptr) {
        if (paintingToolPreviewWidget->parent() != this) {
            paintingToolPreviewWidget->setParent(this);
        }
        if (!paintingToolPreviewWidget->isVisible()) {
            paintingToolPreviewWidget->show();
        }
        if (QWidget::rect().contains(event->pos())) {   // todo: not working correctly when pressed and outside of canvas bounds
            paintingToolPreviewWidget->move(event->pos().x() - paintingToolPreviewWidget->width() / 2, event->pos().y() - paintingToolPreviewWidget->height() / 2);
        }
    }

    if (isDrawing) {
        applyPaintingAction(event->pos());
        previousPoint = event->pos();
        update();
    }
}

void PaintingWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (paintingToolRegistry->getCurrentTool() == nullptr) return;

        isDrawing = false;

        previousPoint = { };

        auto region = Utilities::General::clipRegionToWidgetBounds(affectedRegion.normalized(), QWidget::size());

        pushIntoUndoStack(Utilities::General::getImageCompressedDelta(bitmapSnapshot.data, bitmapSnapshot.region.width(), region));
        delete[] bitmapSnapshot.data; bitmapSnapshot.data = nullptr;   // todo: probably recycle outer area for next time
        auto optimized = undoStack.top().data.size();
        auto unoptimized = pageSize.width() * pageSize.height() * bytesPerPixel;
        qInfo() << optimized << " vs " << unoptimized << " ... ratio(" << ((double)optimized / unoptimized) << ")";
    }

}

void PaintingWidget::enterEvent(QEnterEvent* event) {
    qInfo() << "enter";
    if (paintingToolPreviewWidget = paintingToolRegistry->getCurrentTool()->getPreviewWidget(); paintingToolPreviewWidget != nullptr) {
        paintingToolPreviewWidget->setParent(this);
        paintingToolPreviewWidget->show();
    }
}

void PaintingWidget::leaveEvent(QEvent* event) {
    qInfo() << "leave";
    if (paintingToolPreviewWidget = paintingToolRegistry->getCurrentTool()->getPreviewWidget(); paintingToolPreviewWidget != nullptr) {
        paintingToolPreviewWidget->hide();
        paintingToolPreviewWidget->setParent(nullptr);
    }
}

void PaintingWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Z && (event->modifiers() | Qt::ControlModifier | Qt::ShiftModifier) == event->modifiers()) {
        if (!redoStack.empty()) {
            Delta delta = redoStack.top();
            redoStack.pop();

            undoStack.push(Utilities::General::getImageCompressedDelta(image.bits(), image.width(), delta.region));

            auto size = delta.region.width() * delta.region.height() * bytesPerPixel;
            uint8_t* decompressed = new uint8_t[delta.region.width() * delta.region.height() * bytesPerPixel];
            Utilities::Compression::decompressRunLengthEncoding(delta.data, decompressed, size);

            QPainter painter(&image);
            painter.drawImage(delta.region, QImage(decompressed, delta.region.width(), delta.region.height(), QImage::Format_ARGB32));
            delete[] decompressed; decompressed = nullptr;
            update();
        }
    } else if (event->key() == Qt::Key_Z && event->modifiers() & Qt::ControlModifier) {
        if (!undoStack.empty()) {
            Delta delta = undoStack.top();
            undoStack.pop();

            redoStack.push(Utilities::General::getImageCompressedDelta(image.bits(), image.width(), delta.region));

            auto size = delta.region.width() * delta.region.height() * bytesPerPixel;
            uint8_t* decompressed = new uint8_t[delta.region.width() * delta.region.height() * bytesPerPixel];
            Utilities::Compression::decompressRunLengthEncoding(delta.data, decompressed, size);

            QPainter painter(&image);
            painter.drawImage(delta.region, QImage(decompressed, delta.region.width(), delta.region.height(), QImage::Format_ARGB32));
                    /* visualize */   painter.drawRect(delta.region.x(), delta.region.y(), delta.region.width() - 1, delta.region.height() - 1);
            delete[] decompressed; decompressed = nullptr;
            update();
        }
    } else if (event->key() == Qt::Key_Tab) {
        isErase = !isErase;
    } else if (event->key() == Qt::Key_S && event->modifiers() & Qt::ControlModifier) {
        QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Image", "", "Images (*.png);;All Files (*)");
        image.save(fileName, "PNG");
    }
}

void PaintingWidget::applyPaintingAction(const QPoint& point) {

    QPainter imagePainter(&image);

    if (!previousPoint.has_value()) {
        auto affectedRect = paintingToolRegistry->getCurrentTool()->perform(&imagePainter, point);
        affectedRegion = affectedRegion.united(affectedRect);
    } else {
        const float segmentLength = 10.0f;
        float dist = Utilities::General::distance(previousPoint.value(), point);
        int segments = static_cast<int>(dist / segmentLength) + 1;
        for (int j = 0; j <= segments; ++j) {
            float t = static_cast<float>(j) / segments;
            QPoint p = Utilities::General::interpolate(previousPoint.value(), point, t);
            auto affectedRect = paintingToolRegistry->getCurrentTool()->perform(&imagePainter, p);
            affectedRegion = affectedRegion.united(affectedRect);
        }
    }
}

void PaintingWidget::pushIntoUndoStack(const Delta& delta) {
    undoStack.push(delta);
    while (!redoStack.empty()) {
        redoStack.pop();
    }
}

Snapshot PaintingWidget::getImageSnapshot(uint8_t* source, int sourceWidth, const QRect& region) {
    auto snapshot = Snapshot();
    snapshot.region = region;
    snapshot.data = new uint8_t[region.width() * region.height() * bytesPerPixel];
    std::memcpy(snapshot.data, image.bits(), region.width() * region.height() * bytesPerPixel);         // naive snapshot
    // Utilities::General::getSubRectangle(snapshot.data, source, sourceWidth, region);
    return snapshot;
}


