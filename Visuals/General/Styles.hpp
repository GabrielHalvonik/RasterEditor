#pragma once

#include <QWidget>
#include <QStyle>

#include "../../Utilities/General.hpp"

namespace Styles {

    struct {
        string name { "Visibility" };

        struct {
            string Visible { "Visible" };
            string Hidden { "Hidden" };
        } Values;

    } Visibility;

    struct {
        string name { "Hovered" };

        struct {
            string True { "True" };
            string False { "False" };
        } Values;

    } Hovered;

    struct {
        string name { "TextLength" };

        struct {
            string Empty { "Empty" };
            string Fine { "Fine" };
            string Long { "Long" };
        } Values;

    } TextLength;

    struct {
        string name { "Fulfillment" };

        struct {
            string Empty { "Empty" };
            string Filled { "Filled" };
        } Values;

    } Fulfillment;

}

namespace Styles {

    template <typename T>
    concept StyledProperty = requires(T style) {
        { style.name } -> std::same_as<string&>;
    };

    template <StyledProperty T>
    void set(QWidget* widget, const T& t, const string& value) {
        widget->setProperty(t.name.c_str(), value.c_str());
        widget->style()->polish(widget);
    }
}
