#sample CMakeLists.txt

cmake_minimum_required(VERSION 2.6)

  PROJECT(grid-solver)

  SET(SOURCES
          main.cpp
          cell.cpp
          grid.cpp
          parameters.cpp
          solver.cpp
          )

  ADD_EXECUTABLE(grid-solver ${SOURCES})

  TARGET_LINK_LIBRARIES(grid-solver)
