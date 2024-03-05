#!/bin/sh

WORKING_DIR=${1:-..}
TARGET_NAME=${2:-libgfx}
SOURCES_VAR=${TARGET_NAME}_SOURCES

cd "$WORKING_DIR"
mkdir CMake

# Using sed to escape filenames with spaces in them.
BASE_SOURCES=$( find sources -type f -name "*.cpp" -or -name "*.h" -or -name "*.hpp" -or -name "*.glsl"| sed 's/ /\\ /g')


echo "

set($SOURCES_VAR
	$BASE_SOURCES
	)" > CMake/CMakeLists_sources_lib.cmake