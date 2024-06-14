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
 
project("flecs_sandbox")
 

FILE(GLOB AllModulesSources modules/*.cpp)
FILE(GLOB AllModulesHeaders modules/*.h)

LIST(REMOVE_ITEM AllSources main.cpp)

add_executable(sandbox main.cpp ${AllModulesSources})
target_link_libraries(sandbox flecs::flecs_static glm::glm spdlog)

add_executable(min1 m1_main.cpp ${AllModulesSources})
target_link_libraries(min1 flecs::flecs_static glm::glm spdlog)

set_property(TARGET sandbox PROPERTY CXX_STANDARD 20)

