#pragma once

#pragma once

#include <QLabel>
#include <QStyle>

#include "../General/Styles.hpp"
#include "../../Utilities/General.hpp"

namespace Visuals::Custom {

    struct Label : QLabel {

        parametrize (Label) {
            parameter <int> { affect (Label::setFixedWidth) } width;
            parameter <int> { affect (Label::setFixedWidth) } height;
            parameter <const QString&> { affect (Label::setText) } text;
        };

        Label() {
            Styles::set(this, Styles::TextLength, Styles::TextLength.Values.Empty);
            Styles::set(this, Styles::Hovered, Styles::Hovered.Values.False);
            Styles::set(this, Styles::Visibility, Styles::Visibility.Values.Hidden);
            // this->style()->polish(this);
            this->setObjectName("Label");
        }

        virtual ~Label() { }

    protected:
        void setText(const QString& text) {
            if (text.length() == 0) {
                Styles::set(this, Styles::TextLength, Styles::TextLength.Values.Empty);
            } else if (text.length() < 10) {
                Styles::set(this, Styles::TextLength, Styles::TextLength.Values.Fine);
            } else {
                Styles::set(this, Styles::TextLength, Styles::TextLength.Values.Long);
                QLabel::setText(text.sliced(0, 10)); return;
            }
            QLabel::setText(text);
        }

        void enterEvent(QEnterEvent *event) override {
            Styles::set(this, Styles::Hovered, Styles::Hovered.Values.True);
        }

        void leaveEvent(QEvent *event) override {
            Styles::set(this, Styles::Hovered, Styles::Hovered.Values.False);
        }
    };

}
