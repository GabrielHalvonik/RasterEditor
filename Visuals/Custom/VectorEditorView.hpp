#pragma once

#include <QGraphicsView>
#include <QResizeEvent>

#include "../../Utilities/General.hpp"

namespace Visuals::Custom {

    struct VectorEditorView : QGraphicsView {

        parametrize (VectorEditorView) {
            parameter <QWidget*> { affect (QWidget::setParent) } parent;
            parameter <QGraphicsScene*> { affect (QGraphicsView::setScene) } scene;
            parameter <const list<QWidget*>&> { affect (VectorEditorView::addMultipleWidget) } items;
            handler <const QSize&> { attach (VectorEditorView::sizeChanged) } sizeChanged;
        };

        VectorEditorView() {
            this->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
            this->setBackgroundBrush(QBrush(QColor::fromRgb(35, 35, 35)));
            setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
            setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        }

        virtual ~VectorEditorView() { }



        void addMultipleWidget(const list<QWidget*>& widgets) {
            for (QWidget* widget : widgets) {
                widget->setParent(this);
            }
        }

    protected:
        void resizeEvent(QResizeEvent* event) override {
            QGraphicsView::resizeEvent(event);
            sizeChanged(event->size());
        }

        void drawBackground(QPainter* painter, const QRectF& rect) override {
            QGraphicsView::drawBackground(painter, rect);

            painter->fillRect(scene()->sceneRect(), QColor::fromRgb(55, 55, 60));

            painter->setPen(QPen(QBrush(QColor::fromRgb(100, 100, 100)), 2, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap));

            for (int i = 25; i < this->scene()->width(); i += 50) {
                for (int j = 25; j < this->scene()->height(); j += 50) {
                    if ((i > rect.x() and i < rect.x() + rect.width()) and (j > rect.y() and j < rect.y() + rect.height())) {
                        painter->drawPoint(i, j);
                    }
                }
            }
        }

    private:
        Event<const QSize&> sizeChanged;
    };

}

