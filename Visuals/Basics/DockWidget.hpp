#pragma once

#include <QDockWidget>
#include <QLabel>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct DockWidget : QDockWidget {

        parametrize (DockWidget) {
            parameter <QWidget*> { affect (QDockWidget::setWidget) } layout;
        };

        DockWidget() {
            // this->setContentsMargins(10, 10, 10, 10);

            this->setFixedWidth(48);
            this->setTitleBarWidget(new QWidget());
            this->setFeatures(DockWidgetFeature::NoDockWidgetFeatures);
        }

        virtual ~DockWidget() { }

    };

}
