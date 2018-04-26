CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Script)

#include_directories(${CMAKE_SOURCE_DIR}/automation/custom)
INCLUDE(${PROJECT_SOURCE_DIR}/../../automation/custom/CopyScripts.cmake)

# List all source files
SET(SOURCE_FILES
    example.py
    texture.py
    tile.py
)

ADD_CUSTOM_TARGET(${PROJECT_NAME} ALL)

COPY_FILES_INTO_DIRECTORY_IF_CHANGED(${PROJECT_SOURCE_DIR} "${SOURCE_FILES}" ${EXECUTABLE_OUTPUT_PATH} ${PROJECT_NAME})
