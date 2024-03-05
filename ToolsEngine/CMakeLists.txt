#TOOLS_ENGINE
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/CMakeLists_sources_lib.cmake)
set(TOOLS_ENGINE ToolsEngine)
set(IMGUI_SOURCES ${PROJECT_SOURCE_DIR}/vendor/imgui)

add_library(${TOOLS_ENGINE} SHARED ${ToolsEngine_SOURCES})


set(ToolsEngine_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sources)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${TOOLS_ENGINE}_SOURCES})

include(GenerateExportHeader)
generate_export_header(${TOOLS_ENGINE}
		BASE_NAME ${TOOLS_ENGINE}
		EXPORT_MACRO_NAME ${TOOLS_ENGINE}_API
		EXPORT_FILE_NAME ${TOOLS_ENGINE}_Export.h
		STATIC_DEFINE ${TOOLS_ENGINE}_STATIC)

if(MSVC)
target_compile_options(${TOOLS_ENGINE} PRIVATE /W4 /WX)
endif()

target_include_directories(${TOOLS_ENGINE} PUBLIC ${ToolsEngine_DIR}
${PROJECT_SOURCE_DIR}/build/ToolsEngine
${IMGUI_SOURCES}
${IMGUI_SOURCES}/backends)

#set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)

if(MSVC)
target_compile_definitions(${TOOLS_ENGINE} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif()

#target_include_directories(${TOOLS_ENGINE} PUBLIC ${VENDOR_DIR}/glad45/include
#PUBLIC ${VENDOR_DIR}/STB)

#target_sources(${TOOLS_ENGINE} PUBLIC ${VENDOR_DIR}/glad45/src/glad.c)