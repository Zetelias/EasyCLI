cmake_minimum_required(VERSION 3.0.0)
project(EasyCLI VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()
add_executable(EasyCliTest test.cpp)
add_test(NAME EasyCliTest COMMAND EasyCliTest)

# Find and add the GTest library
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Link the GTest library
target_link_libraries(EasyCliTest ${GTEST_LIBRARIES} pthread)


add_library(EasyCLI EasyCLI.hpp
        test.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
