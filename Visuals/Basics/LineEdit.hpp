#pragma once

#include <QLineEdit>
#include <QValidator>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct LineEdit : QLineEdit {

        parametrize (LineEdit) {
            parameter <const QString&> { affect (QLineEdit::setText) } text;
            parameter <QValidator*> { affect (LineEdit::setValidator) } validator;
            connection <const QString&, QLineEdit> { subscribe (QLineEdit::textEdited) } textChanged;
        };

        LineEdit() : QLineEdit() { }

        virtual ~LineEdit() { }



        void setValidator(QValidator* validator) {
            validator->setParent(this);
            QLineEdit::setValidator(validator);
        }

    };

}
