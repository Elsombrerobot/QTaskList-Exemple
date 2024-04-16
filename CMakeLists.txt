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

# --------- Qt6 ---------

# Find qt6 and needed components
find_package(Qt6 REQUIRED COMPONENTS
	Widgets
	Network
	Core
	Gui
)

# Setup Qt project
qt_standard_project_setup()

# Set app icons resources and compile the .qrc file into a .cpp file
set(APP_ICON_RESOURCE_QRC "${CMAKE_CURRENT_SOURCE_DIR}/resources/qrc/icons.qrc")
qt6_add_resources(APP_ICON_RESOURCE_CPP ${APP_ICON_RESOURCE_QRC})

qt_add_executable($ENV{PRJ_PROJECT_NAME}
    src/main.cpp
	src/version.h
	
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
	src/widgets/loading_button.h
	src/widgets/loading_button.cpp
	src/widgets/task_loader.h
	src/widgets/task_loader.cpp
	src/widgets/task_browser.h
	src/widgets/task_browser.cpp
	src/widgets/task_menu.h
	src/widgets/task_menu.cpp
	
	# App resources
	${APP_ICON_RESOURCE_CPP}
)

# Prevent the creation of a console window on Windows in release mode.
 set_target_properties($ENV{PRJ_PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
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
	Qt6::Core
	Qt6::Gui
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

