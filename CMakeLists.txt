cmake_minimum_required(VERSION 3.5)

project(AkademeMateQt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Set the path to the UI files
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/src")

# Source and UI files
set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/StudyTimer.cpp
    src/StudyTimer.h
    src/DbInputWindow.h
    src/DbInputWindow.cpp
    src/DbManager.h
    src/DbManager.cpp
    src/Module.h
    src/Module.cpp
    src/Calculations.h
    src/Calculations.cpp
)

set(PROJECT_UIS
    src/MainWindow.ui
    src/DbInputWindow.ui
)

add_subdirectory(src/sqlimport)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AkademeMateQt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    # Generate UI headers
    qt6_wrap_ui(PROJECT_UI_HEADERS ${PROJECT_UIS})
    target_sources(AkademeMateQt PRIVATE ${PROJECT_UI_HEADERS})
else()
    if(ANDROID)
        add_library(AkademeMateQt SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(AkademeMateQt
            ${PROJECT_SOURCES}
        )
    endif()

    # Generate UI headers for Qt5
    qt5_wrap_ui(PROJECT_UI_HEADERS ${PROJECT_UIS})
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
    target_sources(AkademeMateQt PRIVATE ${PROJECT_UI_HEADERS})
endif()

target_link_libraries(AkademeMateQt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# ... (The rest of your CMakeLists.txt file)
