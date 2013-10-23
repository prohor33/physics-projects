#sample CMakeLists.txt

cmake_minimum_required(VERSION 2.6)

  PROJECT(grid-solver)

  SET(SOURCES
          main.cpp
          cell.cpp
          cell-container.cpp
          step-maker.cpp
          parameters.cpp
          step-maker.cpp
          )

  ADD_EXECUTABLE(grid-solver ${SOURCES})

  TARGET_LINK_LIBRARIES(grid-solver)
