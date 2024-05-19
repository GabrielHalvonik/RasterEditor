#pragma once

#include <QMainWindow>

#include "Utilities/General.hpp"

struct ApplicationWindow : QMainWindow {

    ApplicationWindow();

    virtual ~ApplicationWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    observable <QSize> windowSizeObservable;
    observable <const QSize&> editorViewSizeObservable;
};
