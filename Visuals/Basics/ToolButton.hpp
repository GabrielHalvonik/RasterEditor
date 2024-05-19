#pragma once

#include <QToolButton>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct ToolButton : QToolButton {

        parametrize (ToolButton) {
            parameter <int> { affect (QToolBar::setFixedWidth) } width;
            parameter <int> { affect (QToolBar::setFixedHeight) } height;
        };

        ToolButton() {

        }

        virtual ~ToolButton() { }

    };

}
