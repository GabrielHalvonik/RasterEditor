#pragma once

#include <QProgressBar>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct ProgressBar : QProgressBar {

        parametrize (ProgressBar) {
            parameter <int> { affect (QProgressBar::setValue) } value;
        };

        ProgressBar() { }

        virtual ~ProgressBar() { }

    };

}
