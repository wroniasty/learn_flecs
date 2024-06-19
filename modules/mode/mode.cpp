#include "mode.h"

mod::ModeModule::ModeModule(flecs::world& ecs)
{
	ecs.system("::sys::mode::Update")
		.iter([](flecs::iter &it) {
			auto active_mode = it.world().get_mut<comp::ActiveMode>();
			if (active_mode->mode) {
				active_mode->mode->update();
			}
		});
}

namespace game {
	void Mode::init(flecs::world& ecs) {}
	void Mode::update(flecs::world& ecs) {}
	void Mode::draw(flecs::world& ecs) {}
	void Mode::exit(flecs::world& ecs) {}
	void Mode::pause(flecs::world& ecs) {}

	void DebugInfoMode::draw(flecs::world& ecs) {}

}


	