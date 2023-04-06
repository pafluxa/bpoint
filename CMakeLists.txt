cmake_minimum_required(VERSION 3.12)
# Tell Cmake about where to find scripts to look up for some dependencies
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${CMAKE_SOURCE_DIR}/cmake_modules/")

# do not build if in source directoru
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
# do change/touch source code
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

project(bpoint
        LANGUAGES CXX
        VERSION 5.0
        DESCRIPTION "Compiles BPoint, stand-alone telescope pointing library.")

# enable compilation using standard C++11
set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_C_FLAGS "-O3 -fopenmp")
SET(CMAKE_CXX_FLAGS "-O3 -fopenmp")

# use position independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# debug flags
set(CMAKE_CFLAGS_DEBUG "-g -G -Wall -O0")
set(CMAKE_CXXFLAGS_DEBUG "-g -G -Wall -O0")

# directives to find required packages
# SOFA required
find_package(Sofa REQUIRED)

# flag to enable/disable high precision mode
if(HIGH_PRECISION)
  add_definitions(-DUSE_HIGH_PRECISION)
endif()

# compile main library (libpisco)
add_library(bpoint SHARED
    src/BPoint_v5.cpp
    src/jpl_eph.cpp
    src/ElapsedTime.cpp
)

# include private headers
target_include_directories(bpoint PRIVATE 
    src/
)

# include public headers from dependencies
target_include_directories(bpoint PUBLIC
    ${SOFA_INCLUDE_DIR}
)

# executable: test bpoint
add_executable(BPoint_v5_test.x src/BPoint_v5_test.cpp)
target_include_directories(BPoint_v5_test.x PRIVATE src/)
target_link_libraries(BPoint_v5_test.x 
    bpoint
    ${SOFA_LIBRARIES}
)

# executable: convert ASCII JPL Eph. to binary lookup-table
add_executable(asc2bin.x 
    src/asc2bin.cxx
    src/jpl_eph.cpp 
)

install(TARGETS BPoint_v5_test.x DESTINATION bin)
install(TARGETS asc2bin.x DESTINATION bin)
install(TARGETS bpoint DESTINATION lib)
install(FILES src/BPoint_v5.h DESTINATION include)
install(FILES src/jpl_eph.h DESTINATION include)
install(FILES src/ElapsedTime.h DESTINATION include)