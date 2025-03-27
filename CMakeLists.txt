cmake_minimum_required(VERSION 3.16)
project(branch_prediction_experiment LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output executable
add_executable(main main.cpp)

# Include kaizen if it's part of the project
# If kaizen.h is external and not installed, you may add it manually:
# target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/external/kaizen)

# Add a simple test using output capture
enable_testing()

add_test(NAME OutputFormatTest
         COMMAND main --size 1000 --trials 1)

# Check that output includes expected format
set_tests_properties(OutputFormatTest PROPERTIES
    PASS_REGULAR_EXPRESSION "Predictable Loop \(avg ns\).*Unpredictable Loop \(avg ns\).*Difference")
