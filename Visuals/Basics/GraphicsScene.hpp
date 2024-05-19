#pragma once

#include <QGraphicsScene>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct GraphicsScene : QGraphicsScene {

        parametrize (GraphicsScene) {
            parameter <const list<QGraphicsItem*>&> { affect(GraphicsScene::addGraphicsItems) } items;
        };

        GraphicsScene() { }

        virtual ~GraphicsScene() { }



        void addGraphicsItems(const list<QGraphicsItem*>& items) {
            for (QGraphicsItem* item : items) {
                this->addItem(item);
            }
        }

    };

}


