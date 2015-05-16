cmake_minimum_required(VERSION 3.1)
project(Rubik)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
      main.cpp
      Image.cpp
      Point3D.cpp
      Vector3D.cpp
      Face.cpp
      Cube.cpp)
add_executable(Rubik ${SOURCE_FILES} Rubik.cpp Rubik.h)

IF (APPLE)
   include_directories (/System/Library/Frameworks)
   find_library (GLUT_LIBRARY GLUT)
   find_library (OpenGL_LIBRARY OpenGL)
   mark_as_advanced (GLUT_LIBRARY
                     OpenGL_LIBRARY)
   set (EXTRA_LIBS ${GLUT_LIBRARY} ${OpenGL_LIBRARY})
ENDIF (APPLE)

target_link_libraries(Rubik ${EXTRA_LIBS})