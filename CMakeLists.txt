cmake_minimum_required(VERSION 3.16)
project(kjmmap C)

set(CMAKE_CXX_STANDARD 14)

execute_process(
        COMMAND /usr/bin/php-config --include-dir
        OUTPUT_VARIABLE PHP_SOURCE
)
string(REGEX REPLACE "\n$" "" PHP_SOURCE "${PHP_SOURCE}")

message("Using source directory: ${PHP_SOURCE}")

include_directories(${PHP_SOURCE})
include_directories(${PHP_SOURCE}/main)
include_directories(${PHP_SOURCE}/Zend)
include_directories(${PHP_SOURCE}/TSRM)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(kjmmap kjmmap.c
        kjmmap_arginfo.h
        php_kjmmap.h)