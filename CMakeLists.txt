project(Pherrite)

CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 14)

# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)
# add external build for antlrcpp
include(ExternalAntlr4Cpp)
# add antrl4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS})

# set variable pointing to the antlr tool that supports C++
# this is not required if the jar file can be found under PATH environment
set(ANTLR_EXECUTABLE ${PROJECT_SOURCE_DIR}/external/antlr-4.8-complete.jar)
# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)

# Call macro to add lexer and grammar to your build dependencies.
antlr_target(PherriteLexer ${PROJECT_SOURCE_DIR}/grammar/PherriteLexer.g4 LEXER
             PACKAGE pherrite)

antlr_target(PherriteParser ${PROJECT_SOURCE_DIR}/grammar/PherriteParser.g4 PARSER
             PACKAGE pherrite
             DEPENDS_ANTLR PherriteLexer
             COMPILE_FLAGS -lib ${ANTLR_PherriteLexer_OUTPUT_DIR} -visitor)

# include generated files in project environment
include_directories(${ANTLR_PherriteLexer_OUTPUT_DIR})
include_directories(${ANTLR_PherriteParser_OUTPUT_DIR})

# add generated grammar to phc binary target
add_executable(pherrite main.cpp translation/codegen.cpp translation/types.cpp
               ${ANTLR_PherriteLexer_CXX_OUTPUTS}
               ${ANTLR_PherriteParser_CXX_OUTPUTS})
target_link_libraries(pherrite antlr4_static)
target_link_libraries(pherrite ${llvm_libs})
