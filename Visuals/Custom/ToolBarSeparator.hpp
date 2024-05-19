#pragma once

#pragma once

#include <QFrame>
#include <QMouseEvent>

#include "../../Utilities/General.hpp"

#include "../Basics/Frame.hpp"

enum class Orientation {
    Horizontal ,
    Vertical
};

namespace Visuals::Custom {

    struct SeparatorElement : QFrame {

        parametrize (SeparatorElement) {
            parameter <QWidget*> { affect(QFrame::setParent) } parent;
        };

        SeparatorElement() {
            QWidget::setStyleSheet("QFrame { border-radius: 1px; border: 0px; background-color: rgba(150, 150, 150, 200); }");
        }

        virtual ~SeparatorElement() { }

    };

}

namespace Visuals::Custom {

    struct ToolBarSeparator : QFrame {

        parametrize (ToolBarSeparator) {
            parameter <const QPoint&> { affect(ToolBarSeparator::setOffset) } offset;
            parameter <const QSize&> { affect(ToolBarSeparator::setFrameSize) } frameSize;
            parameter <const QSize&> { affect(ToolBarSeparator::setSeparatorSize) } separatorSize;
            parameter <Orientation> { affect(ToolBarSeparator::setOrientation) } orientation;
        };

        ToolBarSeparator() {
            QWidget::setStyleSheet("QFrame { background-color: rgba(100, 100, 100, 10); }");
            // this->setFrameStyle(orientation == Orientation::Vertical ? QFrame::HLine : QFrame::HLine);
        }

        virtual ~ToolBarSeparator() { }


        void setFrameSize(const QSize& size) {
            QWidget::setGeometry(geometry().x(), geometry().y(), size.width(), size.height());
        }

        void setOffset(const QPoint& point) {
            QWidget::setGeometry(point.x(), point.y(), geometry().width(), geometry().height());
        }

        void setSeparatorSize(const QSize& size) {
            separator->setGeometry((geometry().width() - size.width()) / 2, (geometry().height() - size.height()) / 2, size.width(), size.height());
            // separator.setParent(this);
        }

        inline void setOrientation(Orientation value) { orientation = value; }

    protected:

        void enterEvent(QEnterEvent*) override {
            setCursor(Qt::CursorShape::SplitVCursor);
        }

        void leaveEvent(QEvent*) override {
            unsetCursor();
        }

        void mousePressEvent(QMouseEvent* event) override {
            if (event->button() == Qt::LeftButton) {
                if (rect().contains(event->pos())) {
                    m_dragStartPos = event->pos();
                    m_dragging = true;
                }
            }
        }

        void mouseMoveEvent(QMouseEvent* event) override {
            if (m_dragging && (event->buttons() & Qt::LeftButton)) {
                QPoint delta = event->pos() - m_dragStartPos;
                move(pos().x(), pos().y() + delta.y());
                qInfo() << "DRAGGING";
            }
        }

        void mouseReleaseEvent(QMouseEvent* event) override {
            if (event->button() == Qt::LeftButton) {
                m_dragging = false;
            }
        }

    private:
        Orientation orientation { Orientation::Vertical };
        QPoint m_dragStartPos;
        SeparatorElement* separator = new SeparatorElement( { .parent= this } );
        bool m_dragging = false;
    };

}


