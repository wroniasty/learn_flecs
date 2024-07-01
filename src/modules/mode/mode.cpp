#include "mode.h"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "render_sandbox.h"
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

	ecs.entity("DebugInfo")
		.set<comp::GameMode>({ new game::DebugInfoMode() })
		.child_of(active_mode_entity)
	;

	ecs.entity("RenderSandbox")
		.set<comp::GameMode>({ new game::RenderSandboxMode() })
		.child_of(active_mode_entity)
	;

	ecs.system("::sys::RenderGameMode")
		.kind(flecs::OnStore)
		.iter([this](flecs::iter& it) {
		auto active_mode = this->active_mode_entity.get<comp::GameMode>();
		if (active_mode->ptr) {
			active_mode->ptr->draw(it.world());
		}
	});

}

void mod::ModeModule::setActiveModule(const flecs::world &ecs, std::string name) const
{
	auto current_mode = active_mode_entity.get_mut<comp::GameMode>();
	if (current_mode->ptr) {
		current_mode->ptr->exit(ecs);
	}

	auto mode_entity = active_mode_entity.lookup(name.c_str());
	if (mode_entity) {
		auto mode = mode_entity.get_mut<comp::GameMode>(); 
		if (mode) {
			if (current_mode->ptr) {
				mode->ptr->show_debug_overlay = current_mode->ptr->show_debug_overlay;
				mode->ptr->show_debug_ui = current_mode->ptr->show_debug_ui;
			}
			current_mode->ptr = mode->ptr;
			mode->ptr->init(ecs);
		}
	}

}

namespace game {
	void Mode::init(const flecs::world& ecs) {

	}
	
	void Mode::update(const flecs::world& ecs) {}
	
	void Mode::draw(const flecs::world& ecs) {}

	void Mode::exit(const flecs::world& ecs) {

	}
	void Mode::pause(const flecs::world& ecs) {}

	//void Mode::debug_ui(const flecs::world& ecs, const bgfx::ViewId view) {
	//	input_module = ecs.get_mut<::mod::Input>();
	//	glfw_module = ecs.module<::mod::GLFW>();
	//	auto window_size = glfw_module.get<comp::gfx::WindowSize>();
	//	
	//	if (input_module->actions["ToggleDebugUI"].pressed) {
	//		show_debug_ui = !show_debug_ui;
	//		spdlog::info("UI {}", show_debug_ui);
	//	}
	//	if (input_module->actions["ToggleDebugOverlay"].pressed) {
	//		show_debug_overlay = !show_debug_overlay;
	//		spdlog::info("Overlay {}", show_debug_overlay);
	//	}

	//	uint8_t imgui_mouse_buttons =
	//		(input_module->mouse_buttons[GLFW_MOUSE_BUTTON_LEFT].down ? IMGUI_MBUT_LEFT : 0) |
	//		(input_module->mouse_buttons[GLFW_MOUSE_BUTTON_RIGHT].down ? IMGUI_MBUT_RIGHT : 0) |
	//		(input_module->mouse_buttons[GLFW_MOUSE_BUTTON_MIDDLE].down ? IMGUI_MBUT_MIDDLE : 0)
	//		;
	//	if (show_debug_ui) {
	//		imguiBeginFrame(input_module->mouse_pos.x, input_module->mouse_pos.y,
	//			imgui_mouse_buttons, 0 /* TODO: scroll */, window_size->width, window_size->height, -1, view);
	//		ImGui::Begin("Available modes");	
	//		auto mode_module = ecs.get<::mod::ModeModule>();
	//		mode_module->active_mode_entity.children([&ecs, mode_module](flecs::entity e) {
	//			auto mode = e.get<comp::GameMode>();
	//			if (mode->ptr) {
	//				if (ImGui::Button(e.name().c_str())) {						
	//					mode_module->setActiveModule(ecs, e.name().c_str());
	//				}
	//			}
	//		});
	//		ImGui::End();
	//		imguiEndFrame();
	//	}
	//	else {
	//		bgfx::touch(view);
	//	}
	//	
	//	if (show_debug_overlay) {
	//		bgfx::setDebug(BGFX_DEBUG_STATS);
	//	}
	//	else {
	//		bgfx::setDebug(BGFX_DEBUG_NONE);
	//	}
	//}

	void DebugInfoMode::draw(const flecs::world& ecs) {
		input_module = ecs.get_mut<::mod::Input>();
		glfw_module = ecs.module<::mod::GLFW>();		
	}


}


	