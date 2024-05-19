#pragma once

#include <QLabel>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct Label : QLabel {

        parametrize (Label) {
            parameter <const QString&> { affect (Label::setText) } text;
        };

        Label() { }

        virtual ~Label() { }

    };

}
