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

    QObject::connect(centralWidget, &CentralWidget::sizeChanged, paintingToolBar, &PaintingToolBar::updatePosition);
}

ApplicationWindow::~ApplicationWindow() {
    delete paintingToolRegistry;
    paintingToolRegistry = nullptr;
}

void ApplicationWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
}
