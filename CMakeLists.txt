cmake_minimum_required(VERSION 3.16)

# --------- QTaskListExemple ---------

# Define the project and language
project($ENV{PRJ_PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set custom installation directory in build_windows/bin
set(CMAKE_INSTALL_PREFIX  ${CMAKE_CURRENT_BINARY_DIR}/bin/$ENV{PRJ_PROJECT_NAME} CACHE PATH "Installation prefix." FORCE)


# Set QTaskListExemple to be default on Visual Studio.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT $ENV{PRJ_PROJECT_NAME})

# Prevent the creation of a console window on Windows
# set_target_properties($ENV{PRJ_PROJECT_NAME} PROPERTIES
#    WIN32_EXECUTABLE ON
# )

# --------- Qt6 ---------

# Find Qt6
list(APPEND CMAKE_PREFIX_PATH $ENV{QT_PATH})

# Find qt6 and needed components
find_package(Qt6 REQUIRED COMPONENTS
	Widgets
	Network
)

# Setup Qt project
qt_standard_project_setup()

# Use file(GLOB_RECURSE ...) to generate a list of source files recursively
file(GLOB_RECURSE MAIN_SOURCES src/*/*.cpp src/*/*.h)
qt_add_executable($ENV{PRJ_PROJECT_NAME}
    src/main.cpp
	src/utils/network_utils.h 
	src/utils/network_utils.cpp 
	src/utils/qt_utils.h 
	src/utils/qt_utils.cpp
	src/utils/kitsu_utils.h 
	src/utils/kitsu_utils.cpp
	src/widgets/connection_dialog.h
	src/widgets/connection_dialog.cpp
	src/widgets/main_window.h
	src/widgets/main_window.cpp
)

target_include_directories($ENV{PRJ_PROJECT_NAME} INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/src/utils
	${CMAKE_CURRENT_SOURCE_DIR}/src/widgets
)

# This code takes care of collecting runtime dependencies and copies them to the installation directory.
qt_generate_deploy_app_script(
    TARGET $ENV{PRJ_PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})


# Link qt6 components
target_link_libraries($ENV{PRJ_PROJECT_NAME} PRIVATE
	Qt6::Widgets
	Qt6::Network
)

# --------- QTaskList ---------

# Add QTaskList CMakelists.txt
add_subdirectory(extern/QTaskList)

# Link against QTasklist
target_link_libraries($ENV{PRJ_PROJECT_NAME} PRIVATE QTaskList)

# --------- Install app ---------
install(TARGETS $ENV{PRJ_PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

