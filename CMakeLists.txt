# 设置 CMake 的最低版本要求
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# 设定项目
project(z-buffer)
message(STATUS "[Project] ${PROJECT_NAME}")

# add_definitions("-Wall")

# 修改默认的安装路径前缀
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR} CACHE PATH "install prefix" FORCE)
endif()

# 确定 C++ 版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 默认为realse版本
#SET(CMAKE_BUILD_TYPE "Release")

# 将生成的 exe, dll, lib 等放置在 bin, lib 目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib")

#*********************** 生成依赖库 *************************
#添加所有源代码
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src sources) 

add_library(${PROJECT_NAME}_lib STATIC ${sources})

target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extern/eigen)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extern)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extern/include)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

#*********************** 生成可执行文件 *************************
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui imgui_dir) 
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${imgui_dir}) #${sources})

if(MSVC)
  set_property(TARGET ${PROJECT_NAME} 
             APPEND_STRING 
             PROPERTY LINK_FLAGS "/NODEFAULTLIB:MSVCRT"
             )
endif()

#指定头文件路径
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extern/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extern)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extern/eigen)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extern/lib)

# target_link_libraries(${PROJECT_NAME} PUBLIC  glfw3.lib opengl32.lib)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib glfw3.lib opengl32.lib)
# target_link_libraries(${PROJECT_NAME} PRIVATE glfw3.lib opengl32.lib)