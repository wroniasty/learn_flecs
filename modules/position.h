#pragma once

#include <flecs.h>
#include <glm/glm.hpp>

namespace comp {
	typedef glm::dvec3 Position;

	struct World {};
	struct Local {};

}

namespace mod {
	struct Position {
		Position(flecs::world& ecs);
	};
}


