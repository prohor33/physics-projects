cmake_minimum_required(VERSION 2.6)

PROJECT(grid-solver)

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

include_directories("../include")

SET(SOURCES
        main.cpp
        ci.cpp
        cell.cpp
        grid_file_reader.cpp
        grid.cpp
        flow_keeper.cpp
        integral.cpp
        manager.cpp
        out_result.cpp
        parallel.cpp
        parameters.cpp
        solver.cpp
        test.cpp
        )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")

ADD_EXECUTABLE(grid-solver ${SOURCES})

TARGET_LINK_LIBRARIES(grid-solver ${MPI_LIBRARIES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse2 -msse3")

if(MPI_COMPILE_FLAGS)
  set_target_properties(grid-solver PROPERTIES
                            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
  endif()

if(MPI_LINK_FLAGS)
  set_target_properties(grid-solver PROPERTIES
                            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
