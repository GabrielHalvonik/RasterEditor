#pragma once

#include <QGraphicsEllipseItem>

#include "../../Utilities/General.hpp"

    namespace Visuals::Basics {

    struct GraphicsEllipseItem : QGraphicsEllipseItem {

        parametrize (GraphicsEllipseItem) {
            parameter <const QRectF&> { affect(QGraphicsEllipseItem::setRect) } rectangle;
        };

        GraphicsEllipseItem() { }

        virtual ~GraphicsEllipseItem() { }

    };

}


