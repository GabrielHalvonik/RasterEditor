#pragma once

#include <QToolBar>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct ToolBar : QToolBar {

        parametrize (ToolBar) {
            parameter <int> { affect (QToolBar::setFixedWidth) } width;
            parameter <int> { affect (QToolBar::setFixedHeight) } height;
            parameter <const list<QAction*>&> { affect (ToolBar::addActions) } actions;
        };

        ToolBar() { }

        virtual ~ToolBar() { }



        void addActions(const list<QAction*>& actions) {
            for (QAction* action : actions) {
                QToolBar::addAction(action);
            }
        }

    };

}


