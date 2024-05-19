#pragma once

#pragma once

#include <QToolBar>

#include "../../Utilities/General.hpp"

namespace Visuals::Custom {

struct TopToolBar : QToolBar {

    parametrize (TopToolBar) {
        parameter <int> { affect(QToolBar::setFixedWidth) } width;
        parameter <int> { affect(QToolBar::setFixedHeight) } height;
        parameter <const list<QAction*>&> { affect(TopToolBar::addActions) } actions;
    };

    TopToolBar() {
        QObject::setObjectName("TopToolBar");
        QToolBar::setMovable(false);
        QToolBar::setAllowedAreas(Qt::ToolBarArea::TopToolBarArea);
    }

    virtual ~TopToolBar() { }



    void addActions(const list<QAction*>& actions) {
        for (QAction* action : actions) {
            QToolBar::addAction(action);
        }
    }

};

}


