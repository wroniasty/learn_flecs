#pragma once

#include <flecs.h>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
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

