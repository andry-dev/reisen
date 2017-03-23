cmake_minimum_required(VERSION 2.8)
project(vkfw)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(include)

add_subdirectory(ext/glfw)
include_directories(ext/glfw/include)

add_subdirectory(ext/asl)
include_directories(ext/asl/include)

set(SRC
	src/main.cpp
	src/Window.cpp
	src/Instance.cpp
	src/Device.cpp
	src/Swapchain.cpp
	src/App.cpp)

add_executable(${PROJECT_NAME} ${SRC})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

message(STATUS "[vkfw] Common Flags are: ${CMAKE_CXX_FLAGS}")
message(STATUS "[vkfw] Debug Flags are: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "[vkfw] Release Flags are: ${CMAKE_CXX_FLAGS_RELEASE}")

INCLUDE(FindPkgConfig)

pkg_search_module(GLEW REQUIRED glew)
include_directories(${GLEW_INCLUDE_DIRS})

set(VULKAN_LIBS
	vulkan)

target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES} ${VULKAN_LIBS} glfw asl)
