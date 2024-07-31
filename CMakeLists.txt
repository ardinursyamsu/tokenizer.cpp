cmake_minimum_required(VERSION 3.14)

project(tokenizerllama CXX)

include(FetchContent)

#################### llama.cpp ####################
FetchContent_Declare(
	llama.cpp
	GIT_REPOSITORY https://github.com/ggerganov/llama.cpp.git
	GIT_TAG        b3488
)
FetchContent_MakeAvailable(llama.cpp)

set(TKNLLAMA_DIR ${CMAKE_SOURCE_DIR}/src/main/resources/lib/)

find_package(JNI)

if (JNI_FOUND)
    message(STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message(STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif()

if(NOT JNI_INCLUDE_DIRS)
    message(FATAL_ERROR "Could not determine JNI include directories")
endif()

add_library(tokenizerllama SHARED src/cpp/tokenizer.cpp)

target_include_directories(tokenizerllama PRIVATE src/cpp ${JNI_INCLUDE_DIRS} ${JNI_INCLUDE_DIRS}/win32)
target_link_libraries(tokenizerllama PRIVATE common llama)
target_compile_features(tokenizerllama PRIVATE cxx_std_11)

set_target_properties(tokenizerllama PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${TKNLLAMA_DIR}
)