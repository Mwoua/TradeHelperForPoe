cmake_minimum_required(VERSION 3.19) #3.19 for automatic Qt finalization

project(TradeHelperForPoe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(Qt_Path "C:/Qt/6.8.0/msvc2022_64")
    list(APPEND CMAKE_PREFIX_PATH ${Qt_Path})
else(WIN32)
   set(Qt_Path "/installation/Qt/6.8.0/gcc_64")
   list(APPEND CMAKE_PREFIX_PATH ${Qt_Path})
endif(WIN32)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()
set(CMAKE_AUTORCC true)

qt_add_executable(TradeHelperForPoe "")
qt_disable_unicode_defines(TradeHelperForPoe)

target_link_libraries(TradeHelperForPoe PRIVATE Qt6::Widgets)

target_sources(TradeHelperForPoe
    PRIVATE
        src/main.cpp
        src/version.h

        src/BrowseLineEdit.cpp
        src/BrowseLineEdit.hpp
        src/ClientMonitor.cpp
        src/ClientMonitor.hpp
        src/Settings.cpp
        src/Settings.hpp
        src/SystemTray.cpp
        src/SystemTray.hpp

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
