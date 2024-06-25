#pragma once

#include <flecs.h>
#include <string>
#include <map>

#include "../input.h"
#include <bgfx/bgfx.h>


namespace game {

	struct Mode {
		virtual void init(const flecs::world& ecs);
		virtual void update(const flecs::world& ecs);
		virtual void draw(const flecs::world& ecs);
		virtual void exit(const flecs::world& ecs);
		virtual void pause(const flecs::world& ecs);

		virtual void debug_ui(const flecs::world& ecs, const bgfx::ViewId view = 255L);

		bool show_debug_overlay = false;
		bool show_debug_ui = true;

		mod::Input *input_module = nullptr;
		flecs::entity glfw_module{};
	};

	struct DebugInfoMode : public Mode {
		void draw(const flecs::world& ecs) override;
	};

}

namespace comp {
	
	struct GameMode {
		game::Mode *ptr = nullptr;
	};


}

namespace mod {
	
	struct ModeModule {
		flecs::entity active_mode_entity;

		ModeModule(flecs::world& ecs);

		void setActiveModule(const flecs::world& ecs, std::string name) const;


	};

}