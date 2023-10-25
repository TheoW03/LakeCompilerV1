cmake_minimum_required(VERSION 3.22.1)
project(SimpleProject)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Now build our tools
# Specify the output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/..)

# Name of your output executable
set(EXECUTABLE_NAME output)

# Add your source files to an executable target
add_executable(${EXECUTABLE_NAME} main.cpp 

src/MipsTarget/builtInFunctions.cpp 
src/MipsTarget/ExpressionTree.cpp 
src/MipsTarget/Register.cpp 
src/MipsTarget/MipsTarget.cpp
src/MipsTarget/UtilFunctions.cpp
src/MipsTarget/VaraibleScope.cpp

src/CompilerFrontend/Lexxer.cpp
src/CompilerFrontend/parse.cpp

src/ARMTarget/ARMTarget.cpp
)


# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM libraries
target_link_libraries(${EXECUTABLE_NAME} ${llvm_libs})