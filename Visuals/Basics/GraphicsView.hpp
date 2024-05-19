#pragma once

#include <QGraphicsView>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct GraphicsView : QGraphicsView {

        parametrize (GraphicsView) {
            parameter <QGraphicsScene*> { affect(QGraphicsView::setScene) } scene;
            parameter <const list<QWidget*>&> { affect(GraphicsView::addMultipleWidget) } items;
        };

        GraphicsView() {
            this->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
            this->setBackgroundBrush(QBrush(QColor::fromRgb(35, 35, 35)));
            setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
            setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        }

        virtual ~GraphicsView() { }



        void addMultipleWidget(const list<QWidget*>& widgets) {
            for (QWidget* widget : widgets) {
                widget->setParent(this);
            }
        }

    };

}

