# minimum CMake version required
cmake_minimum_required(VERSION 3.15)
# Project name, version and description
project(gbp-bot VERSION 1.0 DESCRIPTION "A discord bot")

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# Add DPP as dependency
add_subdirectory(libs/DPP)
 
# Create an executable
add_executable(${PROJECT_NAME}
    src/main.cpp
)


# Linking libraries
target_link_libraries(${PROJECT_NAME} 
    dpp
    Python3::Python
)
 
# Specify includes
target_include_directories(${PROJECT_NAME} PRIVATE
    libs/DPP/include
)
 
# Set C++ version
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17 # or 20 if you want something more recent
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)
