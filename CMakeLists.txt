cmake_minimum_required(VERSION 3.1)
project(Rubik)
set(EXECUTABLE_FILE "Rubik.exe")

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Build type defaulting to \"Release\"")
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(UNIX OR APPLE)
	set(CMAKE_CXX_COMPILER "/usr/bin/g++")
	set(CMAKE_CXX_FLAGS "-std=c++11 -g")
ENDIF ()

if(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
endif()

# Defines where all custom CMake scripts go.
set(CMAKE_SCRIPT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
# Defines where all source code goes.
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
# Defines where all shader files go.
set(SHADER_DIR ${SRC_DIR}/shaders)

# Checks if build directory exists, if not, then create one at build time.
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/build/")
	execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/build/)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

# Sets the cmake directory for finding dependencies.
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

####### Dependencies
#set(DEPS ${CMAKE_SOURCE_DIR}/deps)
#set(GLEW_ROOT_DIR ${DEPS}/glew)
#set(GLM_ROOT_DIR ${DEPS}/glm)

#find_package(GLM REQUIRED)

##########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIR})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)
#########################################################


#########################################################
# Include Files
#########################################################
set(COMPILER_ROOT C:/Program\ Files\ \(x86\)/Dev-Cpp/MinGW32)
set(GL_DIR ${COMPILER_ROOT}/include/GL)
set(LIB_DIR ${COMPILER_ROOT}/lib)
set(HEADER_FILES ${GL_DIR}/glu.h ${GL_DIR}/glut.h)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(Rubik ${SOURCE_FILES} ${HEADER_FILES})

########################################################
# Linking & stuff
#########################################################
link_directories($(LIB_DIR))
set(EXTRA_LIBRARIES libglu32.a
                    libglut32.a)
# create the program "Rubik"
target_link_libraries(Rubik ${OPENGL_LIBRARIES} ${EXTRA_LIBRARIES})