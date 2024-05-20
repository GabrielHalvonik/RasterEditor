cmake_minimum_required(VERSION 3.5)

project(RasterEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGLWidgets Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets Svg)

set(PROJECT_SOURCES
        main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RasterEditor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc
        Core/Utilities/General.hpp
        Core/Utilities/Compression.hpp
        Core/Painting/PaintingToolType.hpp
        Core/Painting/PaintingToolRegisty.hpp
        Core/Painting/PaintingToolFactory.hpp
        Core/Painting/PaintingActionBase.hpp Core/Painting/PaintingActionBase.cpp
        Core/Painting/BrushAction.hpp Core/Painting/BrushAction.cpp
        Core/Painting/PenAction.hpp Core/Painting/PenAction.cpp
        Core/Painting/EraserAction.hpp Core/Painting/EraserAction.cpp
        Visuals/PaintingWidget.cpp Visuals/PaintingWidget.hpp
        Visuals/ApplicationWindow.hpp Visuals/ApplicationWindow.cpp
        Visuals/CentralWidget.hpp Visuals/CentralWidget.cpp
        Visuals/PaintingToolBar.hpp Visuals/PaintingToolBar.cpp
        Visuals/Utilities/PaintingToolIconFactory.hpp
        Utilities/Resources.hpp
        Core/Painting/PaintingToolPreviewWidgets.hpp Core/Painting/PaintingToolPreviewWidgets.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET RasterEditor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(RasterEditor SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(RasterEditor
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(RasterEditor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGLWidgets Qt${QT_VERSION_MAJOR}::Svg)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.RasterEditor)
endif()
set_target_properties(RasterEditor PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS RasterEditor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RasterEditor)
endif()
