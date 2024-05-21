#include "ApplicationWindow.hpp"

#include <QToolBar>
#include <QSlider>
#include <QGroupBox>
#include <QListView>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QToolButton>
#include <QDockWidget>
#include <QListWidget>
#include <QProgressBar>
#include <QButtonGroup>
#include <QFormLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QMenuBar>
#include <QMenu>
#include <QDialog>
#include <QLabel>

#include "CentralWidget.hpp"
#include "PaintingWidget.hpp"
#include "PaintingToolBar.hpp"

#include "../Core/Painting/PaintingToolRegisty.hpp"
#include "../Core/Painting/PaintingToolFactory.hpp"

ApplicationWindow::ApplicationWindow() :
    paintingToolRegistry(new PaintingToolRegistry({
        PaintingToolFactory::construct(PaintingToolType::Brush),
        PaintingToolFactory::construct(PaintingToolType::Pen),
        PaintingToolFactory::construct(PaintingToolType::Eraser),
    })),
    centralWidget(new CentralWidget(this)),
    paintingWidget(new PaintingWidget(paintingToolRegistry, { 640, 640 }, this)),
    paintingToolBar(new PaintingToolBar(paintingToolRegistry, this))
{

    QVBoxLayout* layout = new QVBoxLayout();

    QScrollArea* area = new QScrollArea(this);
    area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    area->setAlignment(Qt::AlignmentFlag::AlignCenter);
    area->setWidget(paintingWidget);
    area->setParent(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(area, Qt::AlignmentFlag::AlignCenter);
    centralWidget->setLayout(layout);

    QMainWindow::setCentralWidget(centralWidget);
    paintingWidget->setFocus();
    paintingWidget->setMouseTracking(true);

    paintingToolBar->setParent(centralWidget);

    {   //todo: refactor it...
        auto dock = new QDockWidget(this);
        auto dockWidget = new QWidget(dock);
        auto dockLayout = new QVBoxLayout(dockWidget);

        {
            auto optionsBox = new QGroupBox("options [not connected]");
            optionsBox->setAlignment(Qt::AlignHCenter);
            auto optionsLayout = new QFormLayout(optionsBox);
            {
                optionsLayout->addRow("delta changes:", new QCheckBox(optionsBox));
                optionsLayout->addRow("visual delta undo-redo bounds:", new QCheckBox(optionsBox));
                {
                    auto snapshotMethodBox = new QComboBox(optionsBox);
                    snapshotMethodBox->addItems({"naive", "delta", "tiled"});
                    optionsLayout->addRow("snapshot method:", snapshotMethodBox);
                }
                {
                    auto compressionsBox = new QComboBox(optionsBox);
                    compressionsBox->addItems({"none", "RLE"});
                    optionsLayout->addRow("compression algorithm:", compressionsBox);
                }
            }
            dockLayout->addWidget(optionsBox);
        }

        {
            auto profileBox = new QGroupBox("profile [not connected]");
            profileBox->setAlignment(Qt::AlignHCenter);
            auto optionsLayout = new QVBoxLayout(profileBox);
            optionsLayout->setContentsMargins(0, 0, 0, 0);
            optionsLayout->setSpacing(0);
            for (int i = 0; i < 8; ++i) {
                auto pb = new QProgressBar();
                pb->setValue(std::rand() % 100);
                optionsLayout->addWidget(pb);
            }
            dockLayout->addWidget(profileBox);
        }

        // dockWidget->setLayout(dockLayout);
        dock->setWidget(dockWidget);

        QMainWindow::addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, dock, Qt::Orientation::Vertical);
    }

    auto aboutMenu = new QMenu("About", ApplicationWindow::menuBar());
    auto creditAction = new QAction("Credit", aboutMenu);

    QObject::connect(creditAction, &QAction::triggered, this, &ApplicationWindow::displayCreditDialog);

    aboutMenu->addAction(creditAction);
    ApplicationWindow::menuBar()->addMenu(aboutMenu);

    QObject::connect(centralWidget, &CentralWidget::sizeChanged, paintingToolBar, &PaintingToolBar::updatePosition);
}

ApplicationWindow::~ApplicationWindow() {
    delete paintingToolRegistry;
    paintingToolRegistry = nullptr;
}

void ApplicationWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
}

void ApplicationWindow::displayCreditDialog(bool) {
    QDialog creditDialog; // note: there is either 1 leak of 32b for each dialog display(even empty one), or my leak-tracing utility sucks

    auto dialogLayout = new QVBoxLayout();
    auto box = new QGroupBox("Credit for icons", &creditDialog);
    box->setAlignment(Qt::AlignmentFlag::AlignHCenter);

    auto creditLayout = new QVBoxLayout();
    QLabel *label = new QLabel(box);
    label->setText("<p>Icons used in this application are from <a href=\"https://github.com/free-icons/free-icons\">https://github.com/free-icons/free-icons</a></p>"
                   "<p>Licensed under <a href=\"https://creativecommons.org/licenses/by/4.0/\">Creative Commons Attribution 4.0 International (CC BY 4.0) License</a></p>");
    label->setTextFormat(Qt::RichText);
    label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    label->setOpenExternalLinks(true);

    creditLayout->addWidget(label);
    box->setLayout(creditLayout);
    dialogLayout->addWidget(box);
    creditDialog.setLayout(dialogLayout);

    creditDialog.exec();
}
