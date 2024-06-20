#include <iostream>
#include <flecs.h>

#include "modules/gfx/glfw_bridge.h"
#include "modules/mode/mode.h"

struct  A {
    virtual void x() {
		std::cout << "A::x" << std::endl;
	}
};

struct B : public A {
    void x() override {
		std::cout << "B::x" << std::endl;
	}
};

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
    auto mode_controller = ecs.get<mod::ModeModule>();
    while (!glfwWindowShouldClose(glfw->window))
    {
        ecs.progress();
        auto active_mode = mode_controller->active_mode_entity.get<comp::ActiveMode>();
        if (active_mode->mode) {
            active_mode->mode->draw(ecs);
        }
    }

    glfw_module.remove<comp::gfx::GLFW>();    
    ecs.progress();
    //glfwDestroyWindow(window);
    //glfwTerminate();
}