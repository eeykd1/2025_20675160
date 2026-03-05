cmake_minimum_required(VERSION 3.16)

project(Worksheet6App VERSION 0.1 LANGUAGES CXX)

# --- Qt Automation ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Setup Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# 2. Setup OpenVR Paths
set(OpenVR_INCLUDE_DIR "C:/Users/User/2025_20675160/Worksheet7/Exercise4/OpenVR/headers")
set(OpenVR_LIBRARY "C:/Users/User/2025_20675160/Worksheet7/Exercise4/OpenVR/lib/win64/openvr_api.lib")

# 3. Setup VTK
find_package(VTK REQUIRED)

# 4. Define Source Files
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ModelPart.cpp
    ModelPart.h
    ModelPartList.cpp
    ModelPartList.h
    optiondialog.cpp
    optiondialog.h
    optiondialog.ui
    icons.qrc
)

# 5. Create Executable
qt_add_executable(Worksheet6App ${PROJECT_SOURCES})

# 6. Link Libraries
target_link_libraries(Worksheet6App PRIVATE
    Qt6::Widgets
    ${VTK_LIBRARIES}
    ${OpenVR_LIBRARY}
)

# 7. Copy VR Bindings
add_custom_command(TARGET Worksheet6App POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/vrbindings"
    "$<TARGET_FILE_DIR:Worksheet6App>/vrbindings"
)

# 8. VTK Autoinit (Crucial for rendering)
vtk_module_autoinit(
    TARGETS Worksheet6App
    MODULES ${VTK_LIBRARIES}
)
