#include <iostream>
#include <flecs.h>

#include <imgui.h>
#include <imgui_bgfx.h>


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
    IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGui_ImplGlfw_InitForOpenGL(glfw->window, true);
    //ImGui_ImplOpenGL2_Init();

    auto mode_controller = ecs.get<mod::ModeModule>();

    ecs.system("::sys::Draw")
        .kind(flecs::OnStore)
        .iter([mode_controller](flecs::iter& it) {
            auto active_mode = mode_controller->active_mode_entity.get<comp::GameMode>();
            if (active_mode->ptr) {
                active_mode->ptr->draw(it.world());
            }
	});
    
    mode_controller->setActiveModule(ecs, "DebugInfo");

    while (!glfwWindowShouldClose(glfw->window))
    {
        ecs.progress();
    }

      
    //ImGui_ImplOpenGL2_Shutdown();
    //ImGui_ImplGlfw_Shutdown();
    //ImGui::DestroyContext();


    glfw_module.remove<comp::gfx::GLFW>();    
    //ecs.progress();

}