cmake_minimum_required (VERSION 3.8)
  
include(FetchContent)
 
function(add_git_dependency libName gitURL gitTag)
    FetchContent_Declare(${libName}
        GIT_REPOSITORY ${gitURL}
        GIT_TAG        ${gitTag}
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(${libName})
   # target_compile_optio s(${libName} PRIVATE "-w")
endfunction()

function(copy_resources_to_target target directory)
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
            "$<TARGET_PROPERTY:${target},SOURCE_DIR>/${directory}"
            "$<TARGET_PROPERTY:${target},BINARY_DIR>/${directory}"
	)
endfunction()

add_git_dependency(
    flecs
    https://github.com/SanderMertens/flecs.git
    v3.2.11
) 

add_git_dependency(
    glm  
    https://github.com/g-truc/glm
    bf71a834948186f4097caa076cd2663c69a10e1e #1.0.1
)
 
add_git_dependency(
    spdlog
    https://github.com/gabime/spdlog
    v1.14.1
)  

add_git_dependency(
    catch2
    https://github.com/catchorg/Catch2
    v3.6.0
)

add_git_dependency(
	glfw
    https://github.com/glfw/glfw
    3.4
)

set(BGFX_BUILD_EXAMPLES OFF)


add_git_dependency(
	bgfx
    https://github.com/bkaradzic/bgfx.cmake
    v1.127.8765-472
)

set(BOOST_ROOT "d:/Documents/Games/boost_1_85_0")
find_package(Boost)

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "Boost :${Boost_LIBRARY_DIRS}")
else()
	message(FATAL_ERROR "Boost not found")
endif()

project("flecs_sandbox")

include_directories(imgui/)

FILE(GLOB AllModulesSources modules/*.cpp)
FILE(GLOB AllModulesHeaders modules/*.h)

FILE(GLOB AllGfxModulesSources modules/gfx/*.cpp)
FILE(GLOB AllGfxModulesHeaders modules/gfx/*.h)

FILE(GLOB AllModeModulesSources modules/mode/*.cpp)
FILE(GLOB AllModeModulesHeaders modules/mode/*.h)

FILE(GLOB AllFlecsScripts plecs/*.flecs)
 
LIST(REMOVE_ITEM AllSources main.cpp)

add_custom_target(flecs_scripts DEPENDS ${AllFlecsScripts})
 
FILE(GLOB ImguiSources imgui/*.cpp)

add_subdirectory(resources/shaders)
 
add_executable(runtests "main_test_runner.cpp" ${AllModulesSources})
target_link_libraries(runtests flecs::flecs_static glm::glm spdlog bgfx Catch2)
target_compile_definitions(runtests PUBLIC -D_TEST_SUITE -DGLM_FORCE_SWIZZLE) 
set_property(TARGET runtests PROPERTY CXX_STANDARD 20)

add_executable(scripted "main_scripted.cpp" ${AllModulesSources})
target_link_libraries(scripted flecs::flecs_static glm::glm spdlog bgfx bx bimg glfw)
target_compile_definitions(scripted PUBLIC -DGLM_FORCE_SWIZZLE) 
set_property(TARGET scripted PROPERTY CXX_STANDARD 20)
add_dependencies(scripted flecs_scripts compile_bgfx_shaders)
copy_resources_to_target(scripted plecs)

add_executable(gfx "main_gfx.cpp" 
    ${AllModulesSources} ${AllGfxModulesSources} ${AllModeModulesSources} ${ImguiSources}
    "resources/shaders/shaders.h"
)
target_link_libraries(gfx flecs::flecs_static glm::glm spdlog bgfx bx bimg glfw)
target_compile_definitions(gfx PUBLIC -DGLM_FORCE_SWIZZLE) 
set_property(TARGET gfx PROPERTY CXX_STANDARD 20)
add_dependencies(gfx flecs_scripts compile_bgfx_shaders ${AllShaderHeaders})
copy_resources_to_target(gfx plecs)



  