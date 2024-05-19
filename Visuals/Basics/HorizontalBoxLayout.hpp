#pragma once

#include <QHBoxLayout>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct HorizontalBoxLayout : QHBoxLayout {

        parametrize (HorizontalBoxLayout) {
            parameter <const list<QWidget*>&> { affect(HorizontalBoxLayout::addMultipleWidget) } items;
        };

        HorizontalBoxLayout() { }

        virtual ~HorizontalBoxLayout() { }



        void addMultipleWidget(const list<QWidget*>& widgets) {
            for (QWidget* widget : widgets) {
                this->addWidget(widget);
            }
        }

    };

}
