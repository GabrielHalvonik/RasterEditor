#pragma once

#include <QStatusBar>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct StatusBar : QStatusBar {

        parametrize (StatusBar) {
            parameter <int> { affect(QStatusBar::setFixedHeight) } height;
        };

        StatusBar() { }

        virtual ~StatusBar() { }

    };

}


