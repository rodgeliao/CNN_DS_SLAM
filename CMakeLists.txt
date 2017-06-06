SET(PROJECT_NAME CNN_DS_SLAM)

PROJECT(${PROJECT_NAME})
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)


set(BUILD_TYPE Release)
#set(BUILD_TYPE DEBUG)

set(EXECUTABLE_OUTPUT_PATH bin)
set(LIBRARY_OUTPUT_PATH lib)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#Required libraries
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(G2O REQUIRED)

#Optional libraries

#add flags
add_definitions("-DENABLE_SSE")
set(CMAKE_CXX_FLAGS "${SSE_FLAGS} -std=c++11")

set(dldso_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/util/MatrixType.cpp
    ${PROJECT_SOURCE_DIR}/src/util/DataSetReader.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}//Thirdparty/Sophus/sophus
    ${EIGEN3_INCLUDE_DIR}
)

#compile main add_library
add_library(cnn_ds_slam ${dldso_SOURCE_FILES})

add_executable(cnn_ds_slam_run ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(cnn_ds_slam_run cnn_ds_slam boost_system boost_thread)
