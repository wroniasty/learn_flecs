#pragma once

#include <flecs.h>
#include <string>

namespace comp {
	namespace gfx {
		struct WindowSize {
			int width;
			int height;
		};
		struct WindowTitle {
			std::string title;
		};

		struct TargetFPS {
			int value;
		};

		struct Mouse {};

	}
}


namespace mod {
	struct Gfx {
		Gfx(flecs::world& ecs);
	};
}