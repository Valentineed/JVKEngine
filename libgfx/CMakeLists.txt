#LIBGFX

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/CMakeLists_sources_lib.cmake)

#Set Variables
set(GRAPHIC_LIB libgfx)

set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)
set(GLFW_DIR ${VENDOR_DIR}/glfw3)
set(ASSIMP_DIR ${VENDOR_DIR}/assimp)
set(ASSIMP_LIB ${ASSIMP_DIR}/lib)
set(VULKAN_DIR ${VENDOR_DIR}/vulkan)
set(GLFW3_ROOT ${GLFW_DIR})
set(STB_INCLUDE ${VENDOR_DIR}/STB)

set(GLFW_DLL ${PROJECT_SOURCE_DIR}/vendor/glfw3/lib/glfw3.dll)
set(ASSIMP_DLL ${ASSIMP_LIB}/assimp-vc140-mt.dll)
set(ASSIMP_LIBRARY ${ASSIMP_LIB}/assimp-vc140-mt.lib)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sources)
set(LUA_DIR ${PROJECT_SOURCE_DIR}/vendor/lua54)

set(IMGUI_SOURCES ${PROJECT_SOURCE_DIR}/vendor/imgui)
set(src
    	${IMGUI_SOURCES}/imgui.cpp
	${IMGUI_SOURCES}/imgui_widgets.cpp
    	${IMGUI_SOURCES}/imgui_demo.cpp
    	${IMGUI_SOURCES}/imgui_draw.cpp
	${IMGUI_SOURCES}/imgui_tables.cpp
	${IMGUI_SOURCES}/backends/imgui_impl_glfw.cpp
	${IMGUI_SOURCES}/backends/imgui_impl_vulkan.cpp
)

set(headers
    	${IMGUI_SOURCES}/imgui.h
	${IMGUI_SOURCES}/backends/imgui_impl_glfw.h
	${IMGUI_SOURCES}/backends/imgui_impl_vulkan.h
)

add_library(${GRAPHIC_LIB} SHARED ${libgfx_SOURCES} ${src} ${headers})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/libgfx/Cmake/")

find_package(GLFW3 REQUIRED)

find_package(Vulkan REQUIRED)

if(Vulkan_FOUND)
	target_include_directories(${GRAPHIC_LIB} PRIVATE ${Vulkan_INCLUDE_DIR})
	target_link_libraries(${GRAPHIC_LIB} PRIVATE ${Vulkan_LIBRARY})
endif()



source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${GRAPHIC_LIB}_SOURCES})

include(GenerateExportHeader)
generate_export_header(${GRAPHIC_LIB}
		BASE_NAME ${GRAPHIC_LIB}
		EXPORT_MACRO_NAME ${GRAPHIC_LIB}_API
		EXPORT_FILE_NAME ${GRAPHIC_LIB}_Export.h
		STATIC_DEFINE ${GRAPHIC_LIB}_STATIC)

if(MSVC)
target_compile_options(${GRAPHIC_LIB} PRIVATE /W4 /WX)
endif()



target_include_directories(${GRAPHIC_LIB} PUBLIC 	${LIB_DIR}
							${PROJECT_SOURCE_DIR}/libMaths/source
							${PROJECT_SOURCE_DIR}/build/libgfx
							${PROJECT_SOURCE_DIR}/build/libMaths
							${PROJECT_SOURCE_DIR}/build/ToolsEngine
							${IMGUI_SOURCES}
							${IMGUI_SOURCES}/backends
							${GLFW3_INCLUDE_DIR}
						  	${STB_INCLUDE}
							${ASSIMP_DIR}/include
							${PROJECT_SOURCE_DIR}/vendor/sol/include
							${LUA_DIR}/include
							)

#target_include_directories(${GRAPHIC_LIB} PRIVATE ${GLFW3_INCLUDE_DIR}
						  #${STB_INCLUDE})

target_link_libraries(${GRAPHIC_LIB} PRIVATE ${GLFW3_LIBRARY})
target_link_libraries(${GRAPHIC_LIB} PRIVATE ${MATHS_LIB})
target_link_libraries(${GRAPHIC_LIB} PRIVATE ${TOOLS_ENGINE})
target_link_libraries(${GRAPHIC_LIB} PRIVATE ${ASSIMP_LIBRARY})
target_link_libraries(${GRAPHIC_LIB} PRIVATE ${PROJECT_SOURCE_DIR}/vendor/lua54/lua54.lib)

target_compile_definitions(${GRAPHIC_LIB} PRIVATE "GLFW_INCLUDE_NONE")

add_custom_command(TARGET ${GRAPHIC_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLFW_DLL} $<TARGET_FILE_DIR:${GRAPHIC_LIB}>)
add_custom_command(TARGET ${GRAPHIC_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ASSIMP_DLL} $<TARGET_FILE_DIR:${GRAPHIC_LIB}>)



if(MSVC)
target_compile_definitions(${GRAPHIC_LIB} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif()
