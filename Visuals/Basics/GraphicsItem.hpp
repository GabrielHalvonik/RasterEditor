#pragma once

#include <QGraphicsItem>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct GraphicsItem : QGraphicsItem {

        parametrize (GraphicsItem) {
            parameter <const list<QGraphicsItem*>&> { affect(GraphicsScene::addGraphicsItems) } items;
        };

        GraphicsItem() { }

        virtual ~GraphicsItem() { }

    };

}


