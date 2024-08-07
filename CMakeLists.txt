# Derived from https://shot511.github.io/2018-05-29-how-to-setup-opengl-project-with-cmake/

# General
cmake_minimum_required(VERSION "3.21")
project("Echtzeitgrafik")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)
	
# Add .lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Configure assets header file
configure_file(src/helper/RootDir.h.in src/helper/RootDir.h)
include_directories(${CMAKE_BINARY_DIR}/src)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# Search for additional libraries
find_package(OpenGL REQUIRED)

find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

find_package(GLEW REQUIRED)
message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")

find_package(Freetype REQUIRED)
message(STATUS "Found Freetype in ${FREETYPE_INCLUDE_DIR}")

add_library(STB_IMAGE "thirdparty/stb_image.cpp")

set(LIBS glfw3 opengl32 assimp glew32s freetype STB_IMAGE)

# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
)

# Link the libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})

# Copy dlls
# Only for windows
if(WIN32)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/dll"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()