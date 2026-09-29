cmake_minimum_required(VERSION 3.19)
project(MultiRowTabWidget_Demo LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(MultiRowTabWidget_Demo
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    RowTab.h RowTab.cpp
    MultiRowTabWidget.h MultiRowTabWidget.cpp
    HiddenFirstTabBar.h
)

target_link_libraries(MultiRowTabWidget_Demo
    PRIVATE
        Qt::Core
        Qt::Widgets
)
target_link_libraries(MultiRowTabWidget_Demo PRIVATE Qt6::Core)
target_link_libraries(MultiRowTabWidget_Demo PRIVATE Qt6::Widgets)

include(GNUInstallDirs)

install(TARGETS MultiRowTabWidget_Demo
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MultiRowTabWidget_Demo
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
