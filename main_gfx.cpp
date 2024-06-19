#include <iostream>
#include <flecs.h>

#include "modules/gfx/glfw_bridge.h"
#include "modules/mode/mode.h"

int main(int, char**)
{
    flecs::world ecs;
    ecs.set<flecs::Rest>({}); // enable rest api
    ecs.set_target_fps(60);

    ecs.import<mod::ModeModule>();

    auto glfw_module = ecs.import<mod::GLFW>()
        .set<comp::gfx::GLFW>({})
        .set<comp::gfx::WindowSize>({ 1000, 600 })
        .set<comp::gfx::WindowTitle>({ "GLFW"})
        .set<comp::gfx::TargetFPS>({ 90 })
        ;

    auto glfw = glfw_module.get<comp::gfx::GLFW>();
    
    while (!glfwWindowShouldClose(glfw->window))
    {
        ecs.progress();
        bgfx::touch(0);
        bgfx::setDebug(BGFX_DEBUG_STATS);
        bgfx::frame();
    }

    glfw_module.remove<comp::gfx::GLFW>();    
    ecs.progress();
    //glfwDestroyWindow(window);
    //glfwTerminate();
}