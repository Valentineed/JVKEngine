cmake_minimum_required(VERSION 3.16.3)

set(PROJECT_NAME JVKEngine)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMake/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(GRAPHIC_LIB libgfx)
set(PHYSIC_LIB libphys)
set(MATHS_LIB libMaths)
set(SOUND_LIB libsound)
set(TOOLS_ENGINE ToolsEngine)

add_subdirectory(${PROJECT_SOURCE_DIR}/libgfx)

add_subdirectory(${PROJECT_SOURCE_DIR}/libphys)

add_subdirectory(${PROJECT_SOURCE_DIR}/libMaths)

add_subdirectory(${PROJECT_SOURCE_DIR}/libsound)

add_subdirectory(${PROJECT_SOURCE_DIR}/EngineEditor)

add_subdirectory(${PROJECT_SOURCE_DIR}/ToolsEngine)

#------------------------------------------------------------------------------------------------
	
SET(CMAKE_CXX_FLAGS_SHIPPING
    "-W0 -D_SHIPPING -DNDEBUG"
    CACHE STRING "Flags used by the C++ compiler during Shipping builds."
    FORCE )
SET(CMAKE_C_FLAGS_SHIPPING
    "-W0"
    CACHE STRING "Flags used by the C compiler during Shipping builds."
    FORCE )
SET(CMAKE_EXE_LINKER_FLAGS_SHIPPING
    ""
    CACHE STRING "Flags used for linking binaries during Shipping builds."
    FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_SHIPPING
    ""
    CACHE STRING "Flags used by the shared libraries linker during Shipping builds."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_SHIPPING
    CMAKE_C_FLAGS_SHIPPING
    CMAKE_EXE_LINKER_FLAGS_SHIPPING
    CMAKE_SHARED_LINKER_FLAGS_SHIPPING )
   
set(CMAKE_CONFIGURATION_TYPES Debug Release Shipping CACHE STRING  
INTERNAL FORCE )