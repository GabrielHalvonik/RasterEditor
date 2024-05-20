#include "PaintingWidget.hpp"

#include <QColor>
#include <QTime>
#include <QDebug>
#include <QPainter>
#include <QFileDialog>
#include <QPaintEvent>

#include "../Core/Utilities/General.hpp"
#include "../Core/Utilities/Compression.hpp"
#include "../Core/Painting/PaintingToolRegisty.hpp"

PaintingWidget::PaintingWidget(PaintingToolRegistry* registry, const QSize& pageSize, QWidget* parent) :
    QOpenGLWidget(parent),
    paintingToolRegistry(registry),
    pageSize(pageSize)
{
    QOpenGLWidget::setFixedSize(pageSize);

    image.fill(Qt::white);
    brush.fill(Qt::transparent);

    brush.load(":/resources/brushes/brush.png");

    erase.fill(Qt::transparent);
    QPainter erasePainter { &erase };
    erasePainter.setPen(QColor::fromRgba(0xFFFFFFFF));
    erasePainter.setBrush(QBrush(QColor::fromRgba(0xFFFFFFFF)));
    erasePainter.drawEllipse(0, 0, 50, 50);
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
        if (paintingToolRegistry->getCurrentTool() == nullptr) { qInfo() << "rtrn"; return; }
        isDrawing = true;
        // points.push_back(event->pos());
        affectedRegion = QRect(event->pos(), event->pos());

        bitmapSnapshot = getImageSnapshot(image.bits(), image.width(), image.rect());
        applyPaintingAction(event->pos());
        previousPoint = event->pos();
        update();
    }
}

void PaintingWidget::mouseMoveEvent(QMouseEvent* event) {
    if (isDrawing) {
        applyPaintingAction(event->pos());
        previousPoint = event->pos();
        update();
    }
}

void PaintingWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDrawing = false;

        previousPoint = { };

        pushIntoUndoStack(getImageCompressedDelta(bitmapSnapshot.data, bitmapSnapshot.region.width(), affectedRegion.normalized()));
        delete[] bitmapSnapshot.data; bitmapSnapshot.data = nullptr;   // todo: probably recycle outer area for next time
        auto optimized = undoStack.top().data.size();
        auto unoptimized = pageSize.width() * pageSize.height() * bytesPerPixel;
        qInfo() << optimized << " vs " << unoptimized << " ... ratio(" << ((double)optimized / unoptimized) << ")";
    }

}

void PaintingWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Z && (event->modifiers() | Qt::ControlModifier | Qt::ShiftModifier) == event->modifiers()) {
        if (!redoStack.empty()) {
            Delta delta = redoStack.top();
            redoStack.pop();

            undoStack.push(getImageCompressedDelta(image.bits(), image.width(), delta.region));

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

            redoStack.push(getImageCompressedDelta(image.bits(), image.width(), delta.region));

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
        paintingToolRegistry->getCurrentTool()->perform(&imagePainter, point);
        // imagePainter.drawPixmap(point.x() - brush.width() / 2, point.y() - brush.height() / 2, isErase ? erase : brush);
        QRect currentRect = QRect(point.x() - brush.width() / 2, point.y() - brush.height() / 2, brush.width(), brush.height());
        affectedRegion = affectedRegion.united(currentRect);
    } else {
        const float segmentLength = 10.0f;
        float dist = Utilities::General::distance(previousPoint.value(), point);
        int segments = static_cast<int>(dist / segmentLength) + 1;
        for (int j = 0; j <= segments; ++j) {
            float t = static_cast<float>(j) / segments;
            QPoint p = Utilities::General::interpolate(previousPoint.value(), point, t);
            paintingToolRegistry->getCurrentTool()->perform(&imagePainter, p);
            // imagePainter.drawPixmap(p.x() - brush.width() / 2, p.y() - brush.height() / 2, isErase ? erase : brush);
            QRect currentRect = QRect(p.x() - brush.width() / 2, p.y() - brush.height() / 2, brush.width(), brush.height());
            affectedRegion = affectedRegion.united(currentRect);
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
    Utilities::General::getSubRectangle(snapshot.data, source, sourceWidth, region);
    return snapshot;
}

Delta PaintingWidget::getImageCompressedDelta(uint8_t* source, int sourceWidth, const QRect& region) {
    auto size = region.width() * region.height() * bytesPerPixel;
    uint8_t* data = new uint8_t[size];
    Utilities::General::getSubRectangle(data, source, sourceWidth, region);
    auto result = Utilities::Compression::compressRunLengthEncoding(data, size);
    delete[] data; data = nullptr;
    return Delta({.region = region, .data = result });
}
