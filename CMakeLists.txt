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
   # target_compile_options(${libName} PRIVATE "-w")
endfunction()
  
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
set(OPENGL_VERSION "4.3")
 
add_git_dependency(
    raylib
    https://github.com/raysan5/raylib.git
    5.0
) 

add_git_dependency(
    flecs
    https://github.com/SanderMertens/flecs.git
    v3.2.11
) 

add_git_dependency(
    spdlog
    https://github.com/gabime/spdlog
    v1.14.1
) 

add_git_dependency(
    glm 
    https://github.com/g-truc/glm
    bf71a834948186f4097caa076cd2663c69a10e1e #1.0.1
)

set(BOOST_ROOT "d:/Documents/Games/boost_1_85_0")
#set(BOOST_INCLUDE_DIR "d:\\Documents\\Games\\boost_1_85_0")

find_package(Boost)
 
if(${Boost_FOUND})
    message(STATUS "Found Boost LIBS: ${Boost_LIBRARIES}")
else()
endif()

include_directories(${Boost_INCLUDE_DIRS})

project ("learn_flecs") 
  
FILE(GLOB AllSources *.cpp)
FILE(GLOB AllHeaders *.h)

LIST(REMOVE_ITEM AllSources main.cpp)
 

add_executable (flecs_tools "main.cpp"
		${AllSources}
 )
target_link_libraries(flecs_tools PRIVATE flecs::flecs_static spdlog glm::glm)


set_property(TARGET flecs_tools PROPERTY CXX_STANDARD 20)

if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options(flecs_tools PRIVATE -Wall -Wextra)
endif()
if ( MSVC )
    target_compile_options(flecs_tools PRIVATE /wd26495 /wd4244 )
endif()


if (CMAKE_VERSION VERSION_GREATER 3.12)
  
endif()

# TODO: Add tests and install targets if needed.
  