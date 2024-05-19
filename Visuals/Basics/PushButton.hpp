#pragma once

#include <QPushButton>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct PushButton : QPushButton {

        parametrize (PushButton) {
            parameter <const QString&> { affect (QObject::setObjectName) } name;
            parameter <int> { affect (QWidget::setFixedWidth) } width;
            parameter <int> { affect (QWidget::setFixedHeight) } height;
            parameter <const QString&> { affect (QPushButton::setText) } text;
            connection <bool, QPushButton> { subscribe (QPushButton::clicked) } clicked;
        };

        PushButton() : QPushButton() {

        }

        virtual ~PushButton() { }

    };

}
