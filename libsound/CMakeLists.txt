#LIBSOUND
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/CMakeLists_sources_lib.cmake)

#Set Variables
set(SOUND_LIB libsound)

set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)
set(FMOD_DIR ${VENDOR_DIR}/FMOD)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sources)
set(FMOD_DLL ${FMOD_DIR}/lib/fmod.dll)
set(FMODL_DLL ${FMOD_DIR}/lib/fmodL.dll)

add_library(${SOUND_LIB} SHARED ${libsound_SOURCES})



source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${SOUND_LIB}_SOURCES})

include(GenerateExportHeader)
generate_export_header(${SOUND_LIB}
		BASE_NAME ${SOUND_LIB}
		EXPORT_MACRO_NAME ${SOUND_LIB}_API
		EXPORT_FILE_NAME ${SOUND_LIB}_Export.h
		STATIC_DEFINE ${SOUND_LIB}_STATIC)

if(MSVC)
target_compile_options(${SOUND_LIB} PRIVATE /W4 /WX)
endif()


target_include_directories(${SOUND_LIB} PUBLIC ${LIB_DIR}
${PROJECT_SOURCE_DIR}/build/libsound
${PROJECT_SOURCE_DIR}/build/ToolsEngine
${PROJECT_SOURCE_DIR}/libMaths/source
${PROJECT_SOURCE_DIR}/build/libMaths
${FMOD_DIR}/include)

target_link_libraries(${SOUND_LIB} PRIVATE ${TOOLS_ENGINE})
target_link_libraries(${SOUND_LIB} PRIVATE ${FMOD_DIR}/lib/*.lib)
target_link_libraries(${SOUND_LIB} PRIVATE ${MATHS_LIB})

add_custom_command(TARGET ${SOUND_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FMOD_DLL} $<TARGET_FILE_DIR:${SOUND_LIB}>)
add_custom_command(TARGET ${SOUND_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FMODL_DLL} $<TARGET_FILE_DIR:${SOUND_LIB}>)


if(MSVC)
target_compile_definitions(${SOUND_LIB} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif()

#target_include_directories(${SOUND_LIB} PUBLIC ${VENDOR_DIR}/glad45/include
#PUBLIC ${VENDOR_DIR}/STB)

#target_sources(${SOUND_LIB} PUBLIC ${VENDOR_DIR}/glad45/src/glad.c)