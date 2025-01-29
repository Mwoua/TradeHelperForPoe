cmake_minimum_required(VERSION 3.19) #3.19 for automatic Qt finalization

project(TradeHelperForPoe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(Qt_Path "C:/Qt/6.8.0/msvc2022_64")
    list(APPEND CMAKE_PREFIX_PATH ${Qt_Path})
else(WIN32)
   set(Qt_Path "/installation/Qt/6.8.0/gcc_64")
   list(APPEND CMAKE_PREFIX_PATH ${Qt_Path})
endif(WIN32)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
qt_standard_project_setup()
set(CMAKE_AUTORCC true)

qt_add_executable(TradeHelperForPoe "")
qt_disable_unicode_defines(TradeHelperForPoe)

target_link_libraries(TradeHelperForPoe PRIVATE Qt6::Widgets Qt6::Network)

target_sources(TradeHelperForPoe
    PRIVATE
        src/main.cpp
        src/version.h
        src/Defines.hpp

        src/AboutForm.cpp
        src/AboutForm.hpp
        src/ClientMonitor.cpp
        src/ClientMonitor.hpp
        src/PoeCommand.cpp
        src/PoeCommand.hpp
        src/Settings.cpp
        src/Settings.hpp
        src/StringParser.cpp
        src/StringParser.hpp
        src/SystemTray.cpp
        src/SystemTray.hpp
        src/TradeWidget.cpp
        src/TradeWidget.hpp
        src/TradeWidgetDisplayer.cpp
        src/TradeWidgetDisplayer.hpp
        src/TradeWidgetFactory.cpp
        src/TradeWidgetFactory.hpp
        src/Utils/BrowseLineEdit.cpp
        src/Utils/BrowseLineEdit.hpp
        src/Utils/ElideLabel.hpp
        src/Utils/ElideLabel.cpp

        resources/utf8.manifest # Force utf8 for the app/library if the OS supports it
        resources/template.rc # Set icons and versions of the binary
        resources/resource.qrc
)

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set_property(TARGET TradeHelperForPoe PROPERTY WIN32_EXECUTABLE true) #remove console
endif()

qt_generate_deploy_app_script(
    TARGET TradeHelperForPoe
    OUTPUT_SCRIPT deploy_script
    NO_TRANSLATIONS
)
install(SCRIPT ${deploy_script})
