#include "glfw_bridge.h"
#include <spdlog/spdlog.h>

static void GlfwErrorCallback(int error, const char* description)
{
	spdlog::error("GLFW Error {} : {}", error, description);
}

mod::GLFW::GLFW(flecs::world& ecs)
{
		ecs.component<comp::gfx::GLFW>("::c::gfx::GLFW");

		ecs.import<mod::Gfx>();

		const bgfx::ViewId kClearView = 0;

		ecs.observer<comp::gfx::GLFW>("::observe::GLFW::OnSet")
			.event(flecs::OnSet)
			.each([](flecs::entity e, comp::gfx::GLFW& glfw) {
				spdlog::info("GLFW Initializing");
				if (!glfw.window) {
						glfwSetErrorCallback(GlfwErrorCallback);
						if (!glfwInit())
						{
							return 1;
						}

						glfw.window = glfwCreateWindow(800, 600, "[GLFW]", nullptr, nullptr);
						if (glfw.window == nullptr)
						{
							glfwTerminate();
							return 1;
						}
						bgfx::renderFrame();
						// Initialize bgfx using the native window handle and window resolution.
						bgfx::Init init;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
						init.platformData.ndt = glfwGetX11Display();
						init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(glfw.window);
#elif BX_PLATFORM_OSX
						init.platformData.nwh = glfwGetCocoaWindow(glfw.window);
#elif BX_PLATFORM_WINDOWS
						init.platformData.nwh = glfwGetWin32Window(glfw.window);
#endif
						int width, height;
						glfwGetWindowSize(glfw.window, &width, &height);
						init.resolution.width = (uint32_t)width;
						init.resolution.height = (uint32_t)height;
						init.resolution.reset = BGFX_RESET_VSYNC;
						if (!bgfx::init(init)) {
							spdlog::error("Failed to initialize bgfx");
						}
					}
				}
		);


		ecs.observer<comp::gfx::GLFW>("::observe::GLFW::OnRemove")
			.event(flecs::OnRemove)
			.each([](flecs::entity e, comp::gfx::GLFW& glfw) {
				spdlog::info("GLFW Terminating");
				glfwDestroyWindow(glfw.window);
				glfwTerminate();
			}
		);

		ecs.observer<comp::gfx::WindowSize>("::observe::gfx::WindowSize::OnSet")
			.event(flecs::OnSet)
			.each([](flecs::entity e, comp::gfx::WindowSize& size) {
				spdlog::info("GLFW Setting Window Size to {}x{}", size.width, size.height);
				const comp::gfx::GLFW * glfw = e.get<comp::gfx::GLFW>();
				glfwSetWindowSize(glfw->window, size.width, size.height);
				bgfx::reset((uint32_t)size.width, (uint32_t)size.height, BGFX_RESET_VSYNC);
				bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
			}
		);

		ecs.observer<comp::gfx::WindowTitle>("::observe::gfx::WindowTitle::OnSet")
			.event(flecs::OnSet)
			.each([](flecs::entity e, comp::gfx::WindowTitle& title) {
				spdlog::info("GLFW Setting Window Title to {}", title.title);
				const comp::gfx::GLFW* glfw = e.get<comp::gfx::GLFW>();
				glfwSetWindowTitle(glfw->window, title.title.c_str());				
			}
		);

}
