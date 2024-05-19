#include "Widget.hpp"

#include <QColor>
#include <QTime>
#include <QDebug>
#include <QPainter>
#include <QFileDialog>
#include <QPaintEvent>

#include <QFrame>
#include <QToolBar>
#include <QVBoxLayout>

#include <QActionGroup>
#include <QButtonGroup>
#include <QIcon>
#include <QSvgRenderer>
#include <QToolButton>

#include "../Utilities/General.hpp"
#include "../Utilities/Compression.hpp"

QIcon loadSvgIcon(const QString &svgData, const QSize &size) {
    QSvgRenderer renderer(svgData.toUtf8());
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing, false);
    renderer.render(&painter);

    return QIcon(pixmap);
}

Widget::Widget(QWidget* parent) : QOpenGLWidget(parent) {

    paintingToolRegistry.registerPaintingActions({
        PaintingToolFactory::construct(PaintingToolType::Brush),
        PaintingToolFactory::construct(PaintingToolType::Eraser),
    });

        paintingToolRegistry.setCurrentToolType(PaintingToolType::Brush);

    auto frame = new QFrame();
    frame->setStyleSheet("QFrame { border-radius: 6px; background-color: dark-gray; }");
    auto layout = new QVBoxLayout();
    auto bar = new QToolBar();
    bar->setOrientation(Qt::Orientation::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);

    bar->setStyleSheet("QToolBar { spacing: 0px; border-radius: 6px; background-color: rgb(50, 50, 55); } QToolBar QToolButton { border: 0px; margin: 0px; padding: 0px; } QToolBar QToolButton:checked { background-color: rgb(85, 85, 85); } QToolBar QToolButton[position=\"first\"] { border-top-left-radius: 6px; border-top-right-radius: 6px; } QToolBar QToolButton[position=\"last\"] { border-bottom-left-radius: 6px; border-bottom-right-radius: 6px; }");

    layout->addWidget(bar);
    frame->setLayout(layout);
    // frame->setStyleSheet("QFrame { border-radius: 6px; background-color: red; }");

    // QActionGroup* group = new QActionGroup(this);
    QButtonGroup* group = new QButtonGroup(this);
    group->setExclusive(true);

    {
        QFile file(":/icons/brush.svg");
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&file);
            QIcon icon = loadSvgIcon(stream.readAll(), QSize(20, 20));
            // group->setExclusive(true);
            // group.setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);
            QToolButton* button = new QToolButton();
            button->setFixedSize(32, 32);
            group->addButton(button);
            button->setCheckable(true);
            button->setIcon(icon);
            button->setProperty("position", "first");
            bar->addWidget(button);
            file.close();
        }
    }
    {
        QFile file(":/icons/pen.svg");
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&file);
            QIcon icon = loadSvgIcon(stream.readAll(), QSize(20, 20));
            // group->setExclusive(true);
            // group.setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);
            QToolButton* button = new QToolButton();
            button->setFixedSize(32, 32);
            group->addButton(button);
            button->setCheckable(true);
            button->setIcon(icon);
            bar->addWidget(button);
            file.close();
        }
    }
    {
        QFile file(":/icons/eraser.svg");
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&file);
            QIcon icon = loadSvgIcon(stream.readAll(), QSize(20, 20));
            // group->setExclusive(true);
            // group.setExclusionPolicy(QActionGroup::ExclusionPolicy::Exclusive);
            QToolButton* button2 = new QToolButton();
            button2->setFixedSize(32, 32);
            group->addButton(button2);
            button2->setCheckable(true);
            button2->setIcon(icon);
            button2->setProperty("position", "last");
            bar->addWidget(button2);
            file.close();
        }
    }

    frame->setGeometry(8, 200, 32, group->buttons().size() * 32);
    frame->setParent(this);


    // Create an action with the icon

    // Add the action to the toolbar




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
            u_int8_t* decompressed = new u_int8_t[delta.region.width() * delta.region.height() * bytesPerPixel];
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
            u_int8_t* decompressed = new u_int8_t[delta.region.width() * delta.region.height() * bytesPerPixel];
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

void Widget::drawBrushStroke(const QPoint& point) {

    QPainter imagePainter(&image);

    if (!previousPoint.has_value()) {
        imagePainter.drawPixmap(point.x() - brush.width() / 2, point.y() - brush.height() / 2, isErase ? erase : brush);
        QRect currentRect = QRect(point.x() - brush.width() / 2, point.y() - brush.height() / 2, brush.width(), brush.height());
        affectedRegion = affectedRegion.united(currentRect);
    } else {
        const float segmentLength = 10.0f;
        float dist = Utilities::General::distance(previousPoint.value(), point);
        int segments = static_cast<int>(dist / segmentLength) + 1;
        for (int j = 0; j <= segments; ++j) {
            float t = static_cast<float>(j) / segments;
            QPoint p = Utilities::General::interpolate(previousPoint.value(), point, t);
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

Snapshot Widget::getImageSnapshot(u_int8_t* source, int sourceWidth, const QRect& region) {
    auto snapshot = Snapshot();
    snapshot.region = region;
    snapshot.data = new u_int8_t[region.width() * region.height() * bytesPerPixel];
    Utilities::General::getSubRectangle(snapshot.data, source, sourceWidth, region);
    return snapshot;
}

Delta Widget::getImageCompressedDelta(u_int8_t* source, int sourceWidth, const QRect& region) {
    auto size = region.width() * region.height() * bytesPerPixel;
    u_int8_t* data = new u_int8_t[size];
    Utilities::General::getSubRectangle(data, source, sourceWidth, region);
    auto result = Utilities::Compression::compressRunLengthEncoding(data, size);
    delete[] data; data = nullptr;
    return Delta({.region = region, .data = result });
}
