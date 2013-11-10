cmake_minimum_required(VERSION 2.6)

  PROJECT(grid-solver)

  find_package(MPI REQUIRED)

  include_directories(${MPI_INCLUDE_PATH})

  SET(SOURCES
          main.cpp
          cell.cpp
          grid.cpp
          manager.cpp
          out_result.cpp
          parameters.cpp
          solver.cpp
          )

  ADD_EXECUTABLE(grid-solver ${SOURCES})

  TARGET_LINK_LIBRARIES(grid-solver ${MPI_LIBRARIES})

  if(MPI_COMPILE_FLAGS)
    set_target_properties(grid-solver PROPERTIES
                              COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
    endif()

  if(MPI_LINK_FLAGS)
    set_target_properties(grid-solver PROPERTIES
                              LINK_FLAGS "${MPI_LINK_FLAGS}")
    endif()
