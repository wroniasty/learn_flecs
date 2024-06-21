#pragma once

#include <flecs.h>
#include <string>
#include <map>

namespace game {

	struct Mode {
		virtual void init(const flecs::world& ecs);
		virtual void update(const flecs::world& ecs);
		virtual void draw(const flecs::world& ecs);
		virtual void exit(const flecs::world& ecs);
		virtual void pause(const flecs::world& ecs);
	};

	struct DebugInfoMode : public Mode {
		void draw(const flecs::world& ecs) override;
	};

	struct RenderTestMode : public Mode {
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

		void setActiveModule(const flecs::world& ecs, std::string name);


	};

}