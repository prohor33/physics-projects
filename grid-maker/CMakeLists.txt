cmake_minimum_required(VERSION 2.6)

  PROJECT(grid-maker)

  SET(SOURCES
          grid_maker.cpp
          main.cpp
          )

  ADD_EXECUTABLE(grid-maker ${SOURCES})
