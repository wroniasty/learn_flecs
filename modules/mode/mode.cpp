#include "mode.h"
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>

mod::ModeModule::ModeModule(flecs::world& ecs)
{
	ecs.system("::sys::mode::Update")
		.kind(flecs::OnUpdate)
		.iter([](flecs::iter &it) {
			auto active_mode = it.world().get_mut<comp::ActiveMode>();
			if (active_mode->mode) {
				active_mode->mode->update(it.world());
			}
		});

	active_mode_entity = ecs.entity("::Engine::ActiveMode")
		.set<comp::ActiveMode>({0});

	ecs.entity("DebugInfoMode")
		.set<game::DebugInfoMode>({})
		.child_of(active_mode_entity)
	;

	setActiveModule(ecs, "DebugInfoMode");
}

void mod::ModeModule::setActiveModule(const flecs::world &ecs, std::string name)
{
	auto current_mode = active_mode_entity.get_mut<comp::ActiveMode>();
	if (current_mode->mode) {
		current_mode->mode->exit(ecs);
	}
	// trzeba component z XXMode * (~!!)
	auto mode_entity = active_mode_entity.lookup(name.c_str());
	if (mode_entity) {
		auto mode = mode_entity.get_mut<game::Mode>();  // nie dzia³a dla podklas
		if (mode) {
			current_mode->mode = mode;
			mode->init(ecs);
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
		bgfx::touch(0);
		bgfx::setDebug(BGFX_DEBUG_STATS);
		bgfx::frame();
	}

}


	