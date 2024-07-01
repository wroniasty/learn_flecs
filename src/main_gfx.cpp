#include <iostream>
#include <flecs.h>

#include "modules/gfx/glfw_bridge.h"
#include "modules/mode/mode.h"

#include <glm/gtx/transform.hpp>
#include "modules/mode/render_sandbox.h"


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
        .set<comp::gfx::TargetFPS>({ 25 })
        ;

    auto glfw = glfw_module.get<comp::gfx::GLFW>();
    auto mode_controller = ecs.get<mod::ModeModule>();    
    mode_controller->setActiveModule(ecs, "RenderSandbox");
    long frame = 0;
	    
    while (ecs.progress()) {
    }

    glfw_module.remove<comp::gfx::GLFW>();    
    

}