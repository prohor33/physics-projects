#sample CMakeLists.txt

cmake_minimum_required(VERSION 2.6)

  PROJECT(grid-solver)

  SET(SOURCES
          cell.cpp
          ci.cpp
          edge_conditions.cpp
          grid.cpp
          integral.cpp
          main.cpp
          main_info.cpp
          out_macro_parameters.cpp
          )

  INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})

  ADD_EXECUTABLE(grid-solver ${SOURCES})

  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")

  TARGET_LINK_LIBRARIES(myapp)
