# This is the project CMakeLists.txt file for the test subproject
cmake_minimum_required(VERSION 3.5)

# Include the components directory of the main application:
#
set(EXTRA_COMPONENT_DIRS "../components")

# Set the components to include the tests for.
# This can be overriden from CMake cache:
# - when invoking CMake directly: cmake -D TEST_COMPONENTS="xxxxx" ..
# - when using idf.py: idf.py -T xxxxx build
#
set(TEST_COMPONENTS "tinypico"
                    "rgb_led"
                    "nunchuk"
                    "device_profile"
)

#include_directories("../main/queues")
#include_directories("../main/queues")
#link_directories("../main/queues")
#add_library(funchuk_test ../main/queues.cpp)
#target_include_directories( funchuk_test PRIVATE
#                       "../main/queues" )

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(funchuk_test)
