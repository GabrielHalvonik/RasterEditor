#pragma once

#pragma once

#include <QToolBar>

#include "../../Utilities/General.hpp"

namespace Visuals::Custom {

    struct RightToolBar : QToolBar {

        parametrize (RightToolBar) {
            parameter <int> { affect(QToolBar::setFixedWidth) } width;
            parameter <int> { affect(QToolBar::setFixedHeight) } height;
            parameter <const list<QAction*>&> { affect(RightToolBar::addActions) } actions;
        };

        RightToolBar() {
            QObject::setObjectName("RightToolBar");
            QToolBar::setMovable(false);
            QToolBar::setAllowedAreas(Qt::ToolBarArea::RightToolBarArea);
        }

        virtual ~RightToolBar() { }



        void addActions(const list<QAction*>& actions) {
            for (QAction* action : actions) {
                QToolBar::addAction(action);
            }
        }

    };

}


