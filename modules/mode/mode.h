#pragma once

#include <flecs.h>

namespace game {

	struct Mode {
		virtual void init(flecs::world& ecs);
		virtual void update(flecs::world& ecs);
		virtual void draw(flecs::world& ecs);
		virtual void exit(flecs::world& ecs);
		virtual void pause(flecs::world& ecs);
	};

	struct DebugInfoMode : Mode {
		void draw(flecs::world& ecs) override;
	};

}

namespace comp {
	
	struct ActiveMode {
		game::Mode *mode = nullptr;
	};

}

namespace mod {
	
	struct ModeModule {
		ModeModule(flecs::world& ecs);
	};

}