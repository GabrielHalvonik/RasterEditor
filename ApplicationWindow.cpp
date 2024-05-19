#include "ApplicationWindow.hpp"

#include "Visuals/Basics/Frame.hpp"
#include "Visuals/Custom/Label.hpp"
#include "Visuals/Basics/Widget.hpp"
#include "Visuals/Basics/ToolBar.hpp"
#include "Visuals/Basics/LineEdit.hpp"
#include "Visuals/Basics/StatusBar.hpp"
#include "Visuals/Basics/BoxLayout.hpp"
#include "Visuals/Custom/TopToolBar.hpp"
#include "Visuals/Basics/DockWidget.hpp"
#include "Visuals/Basics/ProgressBar.hpp"
#include "Visuals/Custom/LeftToolBar.hpp"
#include "Visuals/Basics/GraphicsView.hpp"
#include "Visuals/Custom/RightToolBar.hpp"
#include "Visuals/Custom/VectorEditor.hpp"
#include "Visuals/Custom/BottomToolBar.hpp"
#include "Visuals/Basics/GraphicsScene.hpp"
#include "Visuals/Custom/VectorEditorView.hpp"
#include "Visuals/Basics/VerticalBoxLayout.hpp"
#include "Visuals/Basics/HorizontalBoxLayout.hpp"

#include <QResizeEvent>
#include <QFrame>
#include <QStackedLayout>
#include <QPushButton>
#include <QGraphicsBlurEffect>
#include <QGraphicsItem>
#include <QAction>
#include <QList>
#include <QIcon>
#include <QSize>
#include <QStatusBar>
#include <QMenuBar>
#include <QSlider>
#include <QWindow>

using namespace Visuals::Basics;
using namespace Visuals::Custom;

ApplicationWindow::ApplicationWindow() {
    this->setWindowFlag(Qt::WindowType::CustomizeWindowHint,  true);

    this->setStatusBar(new StatusBar ({
        .height = 24
    }));
    // this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea,  new QDockWidget(), Qt::Orientation::Vertical);
    this->setMinimumSize(600, 400);
    // new ToolBar ({  });
    this->addToolBar(Qt::ToolBarArea::TopToolBarArea, new TopToolBar ({
        .height = 34,
        .actions = {
            new QAction (QIcon::fromTheme(QIcon::ThemeIcon::EditUndo), "Undo"),
            new QAction (QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause), "Redo"),
            new QAction (QIcon::fromTheme(QIcon::ThemeIcon::EditRedo), "Redo"),
        }
    }));

    this->addToolBar(Qt::ToolBarArea::RightToolBarArea, new RightToolBar ({
        .width = 300,
        .actions = {
            new QAction (QIcon::fromTheme(QIcon::ThemeIcon::EditUndo), "Undo"),
            new QAction (QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause), "Redo"),
            new QAction (QIcon::fromTheme(QIcon::ThemeIcon::EditRedo), "Redo"),
        }
    }));


    QMainWindow::setCentralWidget(new VectorEditor ());

}

ApplicationWindow::~ApplicationWindow() { }



void ApplicationWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    windowSizeObservable = event->size();
}
