cmake_minimum_required (VERSION 3.8)
  
include(FetchContent)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})

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

set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED OFF)
set(BOX2D_BUILD_DOCS OFF)
 
add_git_dependency(
    box2d
    https://github.com/erincatto/box2d
    v2.4.1
)

add_git_dependency(
    assimp
    https://github.com/assimp/assimp.git
    v5.4.0
) 

set(BOOST_ROOT "d:/Documents/Games/boost_1_85_0")
find_package(Boost)

find_package(Magnum REQUIRED)
find_package(Corrade CONFIG REQUIRED)

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "Boost :${Boost_LIBRARY_DIRS}")
else()
	message(FATAL_ERROR "Boost not found")
endif()

project("flecs_sandbox")


add_subdirectory(corrade EXCLUDE_FROM_ALL)

set(MAGNUM_WITH_GLFWAPPLICATION ON CACHE BOOL "" FORCE)
add_subdirectory(magnum EXCLUDE_FROM_ALL)
 
add_subdirectory(src)