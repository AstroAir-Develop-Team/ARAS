# CMakeLists.txt
#
# Copyright (C) 2020-2022 Max Qian
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>

cmake_minimum_required(VERSION 3.10)
project(AstroAir-Server VERSION 1.0)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake_modules/")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

set(CMAKE_COLOR_MAKEFILE True)

add_executable(aras src/aras.cpp src/aras.hpp)

message("-- Build on ${CMAKE_HOST_SYSTEM}")
message("-- System: ${CMAKE_HOST_SYSTEM_NAME}")
message("-- Platform: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message("-- Version ${CMAKE_HOST_SYSTEM_VERSION}")

if(WIN32)
    message("-- Not supoort Windows")
endif()
if(ANDROID)
    message("-- Not support Andriod")
endif()
if(APPLE)
    message("-- Not support Apple")
endif()

message("-- Use ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} to complier")
message("-- Command line is ${CMAKE_CXX_COMPILE}")

include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)

include_directories(${PROJECT_SOURCE_DIR}/includes/lua)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes/sockpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes/configor)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}includes/spdlog)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes/websocketpp)

include(LinkThirdLib)
include(Utils)

set(ws_version 1.0)
set(socket_version 1.0)
set(modloader_version 1.0)

check_header("pthread.h")
check_header("sys/time.h")
check_header("unistd.h")
check_header("getopt.h")
check_header("string.h")
check_header("stdio.h")
check_header("stdint.h")
check_header("dirent.h")
check_header("zlib.h")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/core/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/core/config.h)

add_library(Ws SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/core/wsserver.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/core/wsserver.hpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes/websocketpp)
link_libraries(Ws libpthread.so)
set_target_properties(Ws PROPERTIES OUTPUT_NAME "ws")
set_target_properties(Ws PROPERTIES VERSION 1.0 SOVERSION 1)
message("-- Libws.so version is ${ws_version}")


add_library(Socket SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/core/socket.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes/sockpp)
set_target_properties(Socket PROPERTIES OUTPUT_NAME "socket")
set_target_properties(Socket PROPERTIES VERSION 1.0 SOVERSION 1)
message("-- Libsocket.so version is ${socket_version}")


add_library(ModLoader ${CMAKE_CURRENT_SOURCE_DIR}/src/core/modloader.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/core/modloader.hpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes/lua)
target_link_libraries(ModLoader ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lua/liblua.a)
set_target_properties(ModLoader PROPERTIES OUTPUT_NAME "modloader")
set_target_properties(ModLoader PROPERTIES VERSION 1.0 SOVERSION 1)
message("-- Libmodloader.so version is ${modloader_version}")

add_library(GUI ${CMAKE_CURRENT_SOURCE_DIR}/src/core/gui.cpp 
                ${CMAKE_CURRENT_SOURCE_DIR}/libraries/ftxui/ftxui-component.a
                ${CMAKE_CURRENT_SOURCE_DIR}/libraries/ftxui/ftxui-dom.a
                ${CMAKE_CURRENT_SOURCE_DIR}/libraries/ftxui/ftxui-screen.a)
add_library(IMGUI   ${CMAKE_CURRENT_SOURCE_DIR}/includes/imgui/imgui.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/includes/imgui/imgui_draw.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/includes/imgui/imgui_tables.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/includes/imgui/imgui_widgets.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/includes/imgui/backend/imgui_impl_glfw.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/includes/imgui/backend/imgui_impl_opengl3.cpp
    )
target_link_libraries(GUI IMGUI)

target_link_libraries(aras Ws)
target_link_libraries(aras ModLoader)
target_link_libraries(aras Socket)
target_link_libraries(aras GUI)

link_libraries(aras ${CMAKE_CURRENT_SOURCE_DIR}/libraries/sockpp/libsockpp.so)
link_libraries(aras libz.so)
link_libraries(aras PUBLIC libglfw.so)
link_libraries(aras PUBLIC libGL.so)
link_libraries(aras PUBLIC libdl.so)

target_include_directories(aras PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/includes")

