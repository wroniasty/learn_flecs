#pragma once

#include <flecs.h>
#include <glm/glm.hpp>

namespace comp {
	typedef glm::dvec3 Vec3d;	
	typedef glm::vec3 Vec3f;

	struct World {};
	struct Local {};
	struct Rotation {};

}

namespace mod {
	struct Position {
		Position(flecs::world& ecs);
	};
}


