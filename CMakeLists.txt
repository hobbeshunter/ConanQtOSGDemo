cmake_minimum_required(VERSION 3.13)
project(ConanQtOSGDemo VERSION 0.1.0 LANGUAGES CXX)

# Configure CCache if available
if (NOT CMAKE_C_COMPILER_LAUNCHER AND NOT CMAKE_CXX_COMPILER_LAUNCHER)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        message(STATUS "Found ccache: ${CCACHE_PROGRAM}")
        set(CMAKE_C_COMPILER_LAUNCHER   ${CCACHE_PROGRAM})
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    else()
        message(STATUS "Could not find ccache")
    endif()
else()
    message(STATUS "Compiler launcher already set. Not configuring ccache.")
    message(STATUS "CMAKE_C_COMPILER_LAUNCHER: ${CMAKE_C_COMPILER_LAUNCHER}")
    message(STATUS "CMAKE_CXX_COMPILER_LAUNCHER: ${CMAKE_CXX_COMPILER_LAUNCHER}")
endif()

if(NOT GENERATOR_IS_MULTI_CONFIG AND NOT CMAKE_BUILD_TYPE )
    message(STATUS "No build type was specified. Will default to Release.")
    set( CMAKE_BUILD_TYPE Release)
endif()

if(NOT CXX_MARCH)
    set(CXX_MARCH native)
endif()

# Include hints to dependencies installed by conan.io
if(EXISTS ${CMAKE_BINARY_DIR}/conan_paths.cmake AND EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    message("Found conan_paths.cmake in ${CMAKE_BINARY_DIR}")
    include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    set(CONAN_SYSTEM_INCLUDES ON) # disables compiler warnings for dependencies
    conan_basic_setup()
    conan_set_libcxx()
    conan_output_dirs_setup()
else()
    message(WARNING "The files conan_paths.cmake and conanbuildinfo.cmake don't exist.
        You have to run conan install first")
endif()

# Look for dependencies
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

find_package(OpenSceneGraph REQUIRED COMPONENTS osgViewer osgGA)

find_package(Eigen3 REQUIRED)

# Define and configure targets
qt_add_executable(ConanQtOSGDemoExe
    MANUAL_FINALIZATION
        src/ConanQTOSGDemo.cpp
        src/widgets/MainWindow.cpp
        src/PointFactory.cpp
        src/utils/QOSGWidget.cpp
        src/views/QAbstractOSGView.cpp
        src/utils/QPickHandler.cpp
        src/utils/PickableGeode.cpp
        src/models/QPointCloudModel.cpp
        src/views/QPointCloudView.cpp
        src/widgets/PointFactoryControl.cpp
        ui/MainWindow.ui
        ui/PointFactoryControl.ui
)

target_compile_features(ConanQtOSGDemoExe PRIVATE cxx_std_17)

# Compiler options
# Warnings
target_compile_options(ConanQtOSGDemoExe PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Werror -pedantic -Wconversion -Wsign-conversion -Woverloaded-virtual -Wshadow>
)

set_target_properties(ConanQtOSGDemoExe PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(ConanQtOSGDemoExe
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(ConanQtOSGDemoExe
   SYSTEM PRIVATE
        ${OPENSCENEGRAPH_INCLUDE_DIRS}
)

target_link_libraries(ConanQtOSGDemoExe
    PRIVATE
        Qt6::Widgets
        Qt6::OpenGLWidgets
        ${OPENSCENEGRAPH_LIBRARIES}
        Eigen3::Eigen
)

qt_finalize_executable(ConanQtOSGDemoExe)

#install (TARGETS ConanQtOSGDemoExe RUNTIME DESTINATION bin)