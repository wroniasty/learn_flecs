#pragma once

#include <flecs.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "gfx.h"

namespace comp {
	namespace gfx {
		struct GLFW {
			GLFWwindow* window;
		};
	}
}

namespace mod {
	struct GLFW {
		GLFW(flecs::world& ecs);
	};
}

