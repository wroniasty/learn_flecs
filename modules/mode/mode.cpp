#include "mode.h"
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <imgui.h>
#include <imgui_bgfx.h>

#include "../input.h"
#include "../gfx/glfw_bridge.h"

mod::ModeModule::ModeModule(flecs::world& ecs)
{
	ecs.system("::sys::mode::Update")
		.kind(flecs::OnUpdate)
		.iter([](flecs::iter &it) {
			auto active_mode = it.world().get_mut<comp::GameMode>();
			if (active_mode->ptr) {
				active_mode->ptr->update(it.world());
			}
		});

	active_mode_entity = ecs.entity("::Engine::ActiveMode")
		.set<comp::GameMode>({0});

	ecs.entity("DebugInfoMode")
		.set<comp::GameMode>({ new game::DebugInfoMode() })
		.child_of(active_mode_entity)
	;

	setActiveModule(ecs, "DebugInfoMode");
}

void mod::ModeModule::setActiveModule(const flecs::world &ecs, std::string name)
{
	auto current_mode = active_mode_entity.get_mut<comp::GameMode>();
	if (current_mode->ptr) {
		current_mode->ptr->exit(ecs);
	}

	auto mode_entity = active_mode_entity.lookup(name.c_str());
	if (mode_entity) {
		auto mode = mode_entity.get_mut<comp::GameMode>(); 
		if (mode) {
			current_mode->ptr = mode->ptr;
			mode->ptr->init(ecs);
		}
	}

}

namespace game {
	void Mode::init(const flecs::world& ecs) {}
	void Mode::update(const flecs::world& ecs) {}
	void Mode::draw(const flecs::world& ecs) {}
	void Mode::exit(const flecs::world& ecs) {}
	void Mode::pause(const flecs::world& ecs) {}

	void DebugInfoMode::draw(const flecs::world& ecs) {
		auto input_module = ecs.get_mut<mod::Input>();
		auto glfw_module = ecs.module<mod::GLFW>();
		auto window_size = glfw_module.get<comp::gfx::WindowSize>();
		
		uint8_t imgui_mouse_buttons = 
			(input_module->mouse_buttons[GLFW_MOUSE_BUTTON_LEFT].down ? IMGUI_MBUT_LEFT : 0) |
			(input_module->mouse_buttons[GLFW_MOUSE_BUTTON_RIGHT].down ? IMGUI_MBUT_RIGHT : 0) |
			(input_module->mouse_buttons[GLFW_MOUSE_BUTTON_MIDDLE].down ? IMGUI_MBUT_MIDDLE : 0)
			;
		bool showDebug=false;
		imguiBeginFrame(input_module->mouse_pos.x, input_module->mouse_pos.y, 
			imgui_mouse_buttons, 0 /* TODO: scroll */, window_size->width, window_size->height, -1, 0);
			ecs.lookup("::Engine::ActiveMode")
				.each([&showDebug](flecs::entity e, comp::GameMode& mode) {
					showDebug = mode.ptr != nullptr;
				});
		imguiEndFrame();
		//ImGui_ImplOpenGL2_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();
		//bgfx::setDebug(BGFX_DEBUG_STATS);
		//ImGui::ShowDemoWindow();
		//ImGui::Render();
		bgfx::touch(1);
		if (showDebug)
			bgfx::setDebug(BGFX_DEBUG_STATS);
		else
			bgfx::setDebug(BGFX_DEBUG_NONE);
		bgfx::frame();
	}

	void RenderTestMode::draw(const flecs::world& ecs) {
		bgfx::touch(0);
		bgfx::frame();
	}

}


	