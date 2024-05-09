cmake_minimum_required(VERSION 3.16)

# --------- QTaskListExemple ---------

# Define the project and language
project($ENV{PRJ_PROJECT_NAME} VERSION 1.0.1 LANGUAGES CXX)
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
	LinguistTools
)

# Setup Qt project
qt_standard_project_setup()

# Set app icons resources and compile the .qrc file into a .cpp file
set(APP_ICON_RESOURCE_QRC "${CMAKE_CURRENT_SOURCE_DIR}/resources/qrc/icons.qrc")
qt6_add_resources(APP_ICON_RESOURCE_CPP ${APP_ICON_RESOURCE_QRC})

set(SOURCES
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
	src/widgets/menu_bar.h
	src/widgets/menu_bar.cpp
	
	# App resources
	${APP_ICON_RESOURCE_CPP}
)

qt_add_executable($ENV{PRJ_PROJECT_NAME} ${SOURCES})

# Translations
# Set translations sources.
set(TS_SOURCES_fr_FR 
	"${CMAKE_CURRENT_SOURCE_DIR}/resources/ts/$ENV{PRJ_PROJECT_NAME}_fr_FR.ts"
	"${CMAKE_CURRENT_SOURCE_DIR}/extern/QTaskList/resources/ts/QTaskList_fr_FR.ts"
)

# Define output merged .ts file
set(MERGED_SOURCE_TS_fr_FR "${CMAKE_CURRENT_BINARY_DIR}/translations/fr_FR.ts")

# Define relative translation directory for build and install
set(QM_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/translations")

# Add custom command to merge .ts files
add_custom_command(
	OUTPUT ${MERGED_SOURCE_TS_fr_FR}
	COMMAND ${CMAKE_COMMAND} -E echo "Merging .ts files into ${MERGED_SOURCE_TS_fr_FR}"
    COMMAND lconvert -i ${TS_SOURCES_fr_FR} -o ${MERGED_SOURCE_TS_fr_FR} 
    DEPENDS ${TS_SOURCES_fr_FR}
    COMMENT "Merging fr_FR .ts files"
)

# Set the directory where .qm files will be generated.
set_source_files_properties(${MERGED_SOURCE_TS_fr_FR}
    PROPERTIES OUTPUT_LOCATION "${QM_INSTALL_DIR}"
)

# Add translation files
qt_add_lrelease($ENV{PRJ_PROJECT_NAME}
	TS_FILES ${MERGED_SOURCE_TS_fr_FR}
	QM_FILES_OUTPUT_VARIABLE QM_FILES
)

# Install translation files to the translation directory
install(FILES ${QM_FILES} DESTINATION ${QM_INSTALL_DIR})

# Prevent the creation of a console window on Windows in release mode.
set_target_properties($ENV{PRJ_PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
)

target_include_directories($ENV{PRJ_PROJECT_NAME} INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/src/utils
	${CMAKE_CURRENT_SOURCE_DIR}/src/widgets
)

# This takes care of collecting runtime dependencies and copies them to the installation directory.
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

