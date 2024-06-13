#pragma once
#include <flecs.h>

namespace comp {
	struct Node {};
	struct SceneRoot {
		flecs::entity_t scene_origin = 0;
	};
}