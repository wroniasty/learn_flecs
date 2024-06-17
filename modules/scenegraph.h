#pragma once
#include <flecs.h>

namespace comp {
	struct Node {};
	struct SceneRoot {
		flecs::entity_t scene_origin = 0;
	};
}

namespace sys {
	flecs::system Register_UpdateTreeWorldPosition(flecs::world& ecs);
}

namespace mod {
	struct SceneGraph {
		SceneGraph(flecs::world& ecs);

		void setSceneRoot(flecs::world &ecs, flecs::entity_t e, flecs::entity_t origin = 0) const;
		void setSceneRootDefer(flecs::world& ecs, flecs::entity_t e, flecs::entity_t origin = 0) const;
	};
}