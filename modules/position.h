#pragma once

#include <flecs.h>
#include <glm/glm.hpp>

namespace comp {
	struct Position : public glm::dvec3 {
		static void Register(flecs::world& ecs);
	};

	struct World {};
	struct Local {};

}