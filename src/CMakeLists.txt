FILE(GLOB AllModulesSources modules/*.cpp)
FILE(GLOB AllModulesHeaders modules/*.h)

FILE(GLOB AllGfxModulesSources modules/gfx/*.cpp)
FILE(GLOB AllGfxModulesHeaders modules/gfx/*.h)

FILE(GLOB AllModeModulesSources modules/mode/*.cpp)
FILE(GLOB AllModeModulesHeaders modules/mode/*.h)

FILE(GLOB AllFlecsScripts plecs/*.flecs)
 
LIST(REMOVE_ITEM AllSources main.cpp)

add_custom_target(flecs_scripts DEPENDS ${AllFlecsScripts})


add_executable(runtests "main_test_runner.cpp" ${AllModulesSources})
target_link_libraries(runtests flecs::flecs_static glm::glm spdlog Catch2)
target_compile_definitions(runtests PUBLIC -D_TEST_SUITE -DGLM_FORCE_SWIZZLE -DGLM_FORCE_LEFT_HANDED) 
set_property(TARGET runtests PROPERTY CXX_STANDARD 20)

add_executable(scripted "main_scripted.cpp" ${AllModulesSources})
target_link_libraries(scripted flecs::flecs_static glm::glm spdlog glfw)
target_compile_definitions(scripted PUBLIC -DGLM_FORCE_SWIZZLE -DGLM_FORCE_LEFT_HANDED) 
set_property(TARGET scripted PROPERTY CXX_STANDARD 20)
copy_resources_to_target(scripted plecs)

add_executable(gfx "main_gfx.cpp" 
    ${AllModulesSources} ${AllGfxModulesSources} ${AllModeModulesSources}
)
target_link_libraries(gfx flecs::flecs_static glm::glm spdlog glfw box2d assimp )
target_compile_definitions(gfx PUBLIC -DGLM_FORCE_SWIZZLE -DGLM_FORCE_LEFT_HANDED) 
set_property(TARGET gfx PROPERTY CXX_STANDARD 20)
copy_resources_to_target(gfx plecs)