# 1. Mandatory version and project name
cmake_minimum_required(VERSION 3.10)
project(BaseStation)

# 2. Qt Settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 3. Find necessary packages
# This finds the VTK library required for the project
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(VTK REQUIRED)

# 4. Define the application and include ALL source files
# Adding modelpart.cpp, modelpartlist.cpp, and optiondialog.cpp fixes
# the LNK2019 "unresolved external symbol" errors.
add_executable(BaseStation
    main.cpp
    mainwindow.cpp
    modelpart.cpp
    modelpartlist.cpp
    optiondialog.cpp
    mainwindow.h
    modelpart.h
    modelpartlist.h
    optiondialog.h
    mainwindow.ui
    optiondialog.ui
)

# 5. Link libraries to the application
# We link both Qt6 Widgets and the VTK libraries found by CMake
target_link_libraries(BaseStation PRIVATE
    Qt6::Widgets
    ${VTK_LIBRARIES}
)

# 6. Copy VR controller bindings to the build folder
# This ensures the app can find controller maps at run-time
add_custom_target(VRBindings)
add_custom_command(TARGET VRBindings PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E
  copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/
)
