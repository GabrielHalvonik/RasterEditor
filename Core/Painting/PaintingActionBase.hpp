#pragma once

#include <QRect>
#include <QSize>
#include <QPoint>

class QWidget;
class QPainter;

struct PaintingActionBase {

    virtual ~PaintingActionBase();

    virtual QRect perform(QPainter*, const QPoint&) = 0;


    inline QWidget* getPreviewWidget() const {
        return previewWidget;
    }

    inline QSize getSize() const {
        return size;
    }

protected:

    inline void setPreviewWidget(QWidget* preview) {
        previewWidget = preview;
    }

    virtual void setSize(const QSize& value) {
        size = value;
    }

private:
    QWidget* previewWidget { };
    QSize size { };
};
