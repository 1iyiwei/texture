CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# library path
INCLUDE_DIRECTORIES(${TextureSrcCode_SOURCE_DIR}/Library)
LINK_DIRECTORIES(${TextureSrcCode_BINARY_DIR}/Library)

SET(LIBS Library)

set(projects
 HelloWorld
 Seam
 Synth
   )

foreach(project ${projects})

PROJECT(${project})
ADD_EXECUTABLE(${project} ${project}.cpp)
TARGET_LINK_LIBRARIES(${project} ${LIBS})

endforeach(project)
