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

set(BOOST_ROOT "d:/Documents/Games/boost_1_85_0")
find_package(Boost)

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "Boost :${Boost_LIBRARY_DIRS}")
else()
	message(FATAL_ERROR "Boost not found")
endif()

project("flecs_sandbox")
 

FILE(GLOB AllModulesSources modules/*.cpp)
FILE(GLOB AllModulesHeaders modules/*.h)

LIST(REMOVE_ITEM AllSources main.cpp)

add_executable(runtests "main_test_runner.cpp" ${AllModulesSources})
target_link_libraries(runtests flecs::flecs_static glm::glm spdlog Catch2)
target_compile_definitions(runtests PUBLIC -D_TEST_SUITE -DGLM_FORCE_SWIZZLE) 
set_property(TARGET runtests PROPERTY CXX_STANDARD 20)


add_executable(scripted "main_scripted.cpp" ${AllModulesSources})
target_link_libraries(scripted flecs::flecs_static glm::glm spdlog)
target_compile_definitions(scripted PUBLIC -DGLM_FORCE_SWIZZLE) 
set_property(TARGET scripted PROPERTY CXX_STANDARD 20)
copy_resources_to_target(scripted plecs)



