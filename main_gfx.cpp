#include <iostream>
#include <flecs.h>

#include "modules/gfx/glfw_bridge.h"

int main(int, char**)
{
    flecs::world ecs;

    auto glfw_module = ecs.import<mod::GLFW>()
        .set<comp::gfx::GLFW>({})
        .set<comp::gfx::WindowSize>({ 1000, 600 })
        .set<comp::gfx::WindowTitle>({ "GLFW"})
        ;

    auto glfw = glfw_module.get<comp::gfx::GLFW>();
    ecs.set_target_fps(60);
    while (!glfwWindowShouldClose(glfw->window))
    {
        glfwPollEvents();
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