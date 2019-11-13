# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(blink_led)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

list(APPEND HEADERS 
    "${PROJECT_SOURCE_DIR}/include"
)
include_directories(${HEADERS})
target_sources(app PRIVATE src/main.c)
