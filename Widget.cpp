#include "Widget.hpp"

#include <QColor>
#include <QTime>
#include <QDebug>
#include <QPainter>
#include <QFileDialog>
#include <QPaintEvent>

float distance(const QPoint& p1, const QPoint& p2) {
    float dx = p2.x() - p1.x();
    float dy = p2.y() - p1.y();
    return std::sqrt(dx * dx + dy * dy);
}

QPoint interpolate(QPoint source, QPoint destination, float percentage) {
    float x = (1 - percentage) * source.x() + percentage * destination.x();
    float y = (1 - percentage) * source.y() + percentage * destination.y();
    return QPoint(static_cast<int>(x), static_cast<int>(y));
}

std::vector<uchar> compressRunLengthEncoding(const uchar* data, int size, int bytesPerPixel = 4) {
    std::vector<uchar> compressed { };
    int i = 0;
    while (i < size) {
        uchar value = data[i];
        int count = 1;
        while (i + count < size && data[i + count] == value && count < 255) {
            ++count;
        }
        compressed.push_back(value);
        compressed.push_back(count);
        i += count;
    }
    return compressed;
}

void decompressRunLengthEncoding(const std::vector<uchar>& compressed, uchar* data, int size) {
    int index = 0;
    int dataIndex = 0;
    while (index < compressed.size() && dataIndex < size) {
        uchar value = compressed[index++];
        uchar count = compressed[index++];
        for (int i = 0; i < count && dataIndex < size; ++i) {
            data[dataIndex++] = value;
        }
    }
}

void Widget::getSubRectangle(uchar* destination, const uchar* source, int sourceWidth, int x, int y, int width, int height) {
    for (int row = 0; row < height; ++row) {
        const uchar* srcRowStart = source + ((y + row) * sourceWidth + x) * bytesPerPixel;
        uchar* destRowStart = destination + row * width * bytesPerPixel;
        std::memcpy(destRowStart, srcRowStart, width * bytesPerPixel);
    }
}

void Widget::getSubRectangle(uchar* destination, const uchar* origin, int originalWidth, const QRect& rect) {
    getSubRectangle(destination, origin, originalWidth, rect.x(), rect.y(), rect.width(), rect.height());
}



Widget::Widget(QWidget* parent) : QOpenGLWidget(parent) {
    image.fill(Qt::white);
    brush.fill(Qt::transparent);

    brush.load(":/brushes/brush.png");

    erase.fill(Qt::transparent);
    QPainter erasePainter { &erase };
    erasePainter.setPen(QColor::fromRgba(0xFFFFFFFF));
    erasePainter.setBrush(QBrush(QColor::fromRgba(0xFFFFFFFF)));
    erasePainter.drawEllipse(0, 0, 50, 50);

}

Widget::~Widget() { }

void Widget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Widget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void Widget::paintGL() {
    QPainter painter(this);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    painter.drawImage(0, 0, image);
}

void Widget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDrawing = true;
        // points.push_back(event->pos());
        affectedRegion = QRect(event->pos(), event->pos());

        bitmapSnapshot = getImageSnapshot(image.bits(), image.width(), image.rect());
        drawBrushStroke(event->pos());
        previousPoint = event->pos();
        update();
    }
}

void Widget::mouseMoveEvent(QMouseEvent* event) {
    if (isDrawing) {
        drawBrushStroke(event->pos());
        previousPoint = event->pos();
        update();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDrawing = false;

        previousPoint = { };

        pushIntoUndoStack(getImageCompressedDelta(bitmapSnapshot.data, bitmapSnapshot.region.width(), affectedRegion.normalized()));
        delete[] bitmapSnapshot.data; bitmapSnapshot.data = nullptr;   // todo: probably recycle outer area for next time
        auto optimized = undoStack.top().data.size();
        auto unoptimized = PageSize * PageSize * bytesPerPixel;
        qInfo() << optimized << " vs " << unoptimized << " ... ratio(" << ((double)optimized / unoptimized) << ")";
    }

}

void Widget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Z && (event->modifiers() | Qt::ControlModifier | Qt::ShiftModifier) == event->modifiers()) {
        if (!redoStack.empty()) {
            Delta delta = redoStack.top();
            redoStack.pop();

            undoStack.push(getImageCompressedDelta(image.bits(), image.width(), delta.region));

            auto size = delta.region.width() * delta.region.height() * bytesPerPixel;
            uchar* decompressed = new uchar[delta.region.width() * delta.region.height() * bytesPerPixel];
            decompressRunLengthEncoding(delta.data, decompressed, size);

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
            uchar* decompressed = new uchar[delta.region.width() * delta.region.height() * bytesPerPixel];
            decompressRunLengthEncoding(delta.data, decompressed, size);

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

void Widget::drawBrushStroke(const QPoint& point) {

    QPainter imagePainter(&image);

    if (!previousPoint.has_value()) {
        imagePainter.drawPixmap(point.x() - brush.width() / 2, point.y() - brush.height() / 2, isErase ? erase : brush);
        QRect currentRect = QRect(point.x() - brush.width() / 2, point.y() - brush.height() / 2, brush.width(), brush.height());
        affectedRegion = affectedRegion.united(currentRect);
    } else {
        const float segmentLength = 10.0f;
        float dist = distance(previousPoint.value(), point);
        int segments = static_cast<int>(dist / segmentLength) + 1;
        for (int j = 0; j <= segments; ++j) {
            float t = static_cast<float>(j) / segments;
            QPoint p = interpolate(previousPoint.value(), point, t);
            imagePainter.drawPixmap(p.x() - brush.width() / 2, p.y() - brush.height() / 2, isErase ? erase : brush);
            QRect currentRect = QRect(p.x() - brush.width() / 2, p.y() - brush.height() / 2, brush.width(), brush.height());
            affectedRegion = affectedRegion.united(currentRect);
        }
    }
}

void Widget::pushIntoUndoStack(const Delta& delta) {
    undoStack.push(delta);
    while (!redoStack.empty()) {
        redoStack.pop();
    }
}

Snapshot Widget::getImageSnapshot(uchar* source, int sourceWidth, const QRect& region) {
    auto snapshot = Snapshot();
    snapshot.region = region;
    snapshot.data = new uchar[region.width() * region.height() * bytesPerPixel];
    getSubRectangle(snapshot.data, source, sourceWidth, region);
    return snapshot;
}

Delta Widget::getImageCompressedDelta(uchar* source, int sourceWidth, const QRect& region) {
    auto size = region.width() * region.height() * bytesPerPixel;
    uchar* data = new uchar[size];
    getSubRectangle(data, source, sourceWidth, region);
    auto result = compressRunLengthEncoding(data, size);
    delete[] data; data = nullptr;
    return Delta({.region = region, .data = result });
}
