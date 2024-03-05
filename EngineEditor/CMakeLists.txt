set(EXE_NAME Engine)


#Set Variable
set(VULKAN_DIR ${VENDOR_DIR}/vulkan)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/EngineEditor/Cmake/")
include(CMake/CMakeLists_sources.cmake)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${${EXE_NAME}_SOURCES})
message( ${${EXE_NAME}_SOURCES} )

set(IMGUI_SOURCES ${PROJECT_SOURCE_DIR}/vendor/imgui)
set(TINYXML_SOURCES ${PROJECT_SOURCE_DIR}/vendor/tinyxml)
set(src
    ${IMGUI_SOURCES}/imgui.cpp
	${IMGUI_SOURCES}/imgui_widgets.cpp
    ${IMGUI_SOURCES}/imgui_demo.cpp
    ${IMGUI_SOURCES}/imgui_draw.cpp
	${IMGUI_SOURCES}/imgui_tables.cpp
	${IMGUI_SOURCES}/backends/imgui_impl_glfw.cpp
	${IMGUI_SOURCES}/backends/imgui_impl_vulkan.cpp
	${TINYXML_SOURCES}/tinyxml.cpp
	${TINYXML_SOURCES}/tinystr.cpp
	${TINYXML_SOURCES}/tinyxmlerror.cpp
	${TINYXML_SOURCES}/tinyxmlparser.cpp
)
set(GLFW_DLL ${PROJECT_SOURCE_DIR}/vendor/glfw3/lib/glfw3.dll)

set(SOL_HEADERDIR ${PROJECT_SOURCE_DIR}/vendor/sol/include)
set(LUA_DIR ${PROJECT_SOURCE_DIR}/vendor/lua54)
set(LUA_LIB ${PROJECT_SOURCE_DIR}/vendor/lua54/lua54.lib)


add_executable(${EXE_NAME} ${${EXE_NAME}_SOURCES} ${src})

set_property(TARGET ${EXE_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

find_package(GLFW3 REQUIRED)

find_package(Vulkan REQUIRED)

if(Vulkan_FOUND)
	target_include_directories(${EXE_NAME} PRIVATE 	${Vulkan_INCLUDE_DIR})
	target_link_libraries(${EXE_NAME} PRIVATE ${Vulkan_LIBRARY})
endif()

#target_link_libraries(${EXE_NAME} PRIVATE ${ENGINE_LIB}
					  #${LIBMATHS_LIB})

target_include_directories(${EXE_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/libgfx/sources
${PROJECT_SOURCE_DIR}/libphys/sources
${PROJECT_SOURCE_DIR}/libsound/sources
${PROJECT_SOURCE_DIR}/libMaths/source
${PROJECT_SOURCE_DIR}/build/libgfx
${PROJECT_SOURCE_DIR}/build/libphys
${PROJECT_SOURCE_DIR}/build/libMaths
${PROJECT_SOURCE_DIR}/build/libsound
${PROJECT_SOURCE_DIR}/build/ToolsEngine
${PROJECT_SOURCE_DIR}/vendor/glfw3/include
${IMGUI_SOURCES}
${IMGUI_SOURCES}/backends
${SOL_HEADERDIR}
${LUA_DIR}/include
${TINYXML_SOURCES}
)

target_link_libraries(${EXE_NAME} PRIVATE ${GRAPHIC_LIB})

target_link_libraries(${EXE_NAME} PRIVATE ${PHYSIC_LIB})

target_link_libraries(${EXE_NAME} PRIVATE ${SOUND_LIB})

target_link_libraries(${EXE_NAME} PRIVATE ${TOOLS_ENGINE})

target_link_libraries(${EXE_NAME} PRIVATE ${MATHS_LIB})

target_link_libraries(${EXE_NAME} PRIVATE ${GLFW3_LIBRARY})

target_link_libraries(${EXE_NAME} PRIVATE ${LUA_LIB})

#target_compile_definitions(${GRAPHIC_LIB} PRIVATE "GLFW_INCLUDE_NONE")


#add_custom_command(TARGET ${EXE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ENGINE_DLL} $<TARGET_FILE_DIR:${EXE_NAME}>)

#add_custom_command(TARGET ${EXE_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLFW_DLL} $<TARGET_FILE_DIR:${EXE_NAME}>)


