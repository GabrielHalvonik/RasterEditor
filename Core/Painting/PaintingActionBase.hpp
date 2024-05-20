#pragma once

#include <QRect>
#include <QPoint>

class QWidget;
class QPainter;

struct PaintingActionBase {

    virtual ~PaintingActionBase();

    virtual QRect perform(QPainter*, const QPoint&) = 0;


    inline QWidget* getPreviewWidget() {
        return previewWidget;
    }

protected:
    inline void setPreviewWidget(QWidget* preview) {
        previewWidget = preview;
    }

private:
    QWidget* previewWidget { };
};
