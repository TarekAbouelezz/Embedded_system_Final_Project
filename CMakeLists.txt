cmake_minimum_required(VERSION 3.10)
project(FlexibleAssemblySystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/Warehouse.cpp
    src/AGV.cpp
    src/AssemblyStation.cpp
    src/ControlCenter.cpp
    src/FileHandler.cpp
)

# Header files
set(HEADERS
    src/Order.h
    src/Product.h
    src/Warehouse.h
    src/AGV.h
    src/AssemblyStation.h
    src/ControlCenter.h
    src/FileHandler.h
)

# Create executable
add_executable(fas_simulator ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(fas_simulator PRIVATE src)

# Link libraries (pthread for multithreading on Unix)
if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(fas_simulator PRIVATE Threads::Threads)
endif()

# Create input/output directories
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/input)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/output)


