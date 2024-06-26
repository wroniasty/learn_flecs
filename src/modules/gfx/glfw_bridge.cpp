#include "glfw_bridge.h"
#include <spdlog/spdlog.h>

#include "../input.h"

static void GlfwErrorCallback(int error, const char* description)
{
	spdlog::error("GLFW Error {} : {}", error, description);
}

struct KeyCallback {
	static void callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		spdlog::info("GLFW Key Callback {} {} {} {}", key, action, scancode, mods);
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
};

KeyCallback key_callback;

mod::GLFW::GLFW(flecs::world& ecs)
{
	ecs.component<comp::gfx::GLFW>("::c::gfx::GLFW");

	ecs.import<mod::Gfx>();
	ecs.import<mod::Input>();

	ecs.observer<comp::gfx::GLFW>("::observe::GLFW::OnSet")
		.event(flecs::OnSet)
		.each([&ecs](flecs::entity e, comp::gfx::GLFW& glfw) {
		spdlog::info("GLFW Initializing");
		if (!glfw.window) {
			glfwSetErrorCallback(GlfwErrorCallback);
			if (!glfwInit())
			{
				return;
			}

			glfw.window = glfwCreateWindow(800, 600, "[GLFW]", nullptr, nullptr);
			if (glfw.window == nullptr)
			{
				glfwTerminate();
				return;
			}

			glfwSetWindowUserPointer(glfw.window, &ecs);
			int width, height;
			glfwGetWindowSize(glfw.window, &width, &height);


			glfwSetKeyCallback(glfw.window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
				flecs::world* ecs = (flecs::world*)glfwGetWindowUserPointer(window);
				auto input_module = ecs->get_mut<mod::Input>();
				if (input_module) {
					//spdlog::info("Keyboard callback");
					comp::inp::KeyState ks = input_module->key_state_map[key];
					if (action == GLFW_PRESS) {
						ks.down = true;
						ks.time_pressed = ecs->get_info()->world_time_total;
						ks.pressed = true;
					}
					else if (action == GLFW_RELEASE) {
						ks.down = false;
						ks.released = true;
					}
					input_module->key_state_map[key] = ks;
				}
				});

			glfwSetMouseButtonCallback(glfw.window, [](GLFWwindow* window, int button, int action, int mods) {
				flecs::world* ecs = (flecs::world*)glfwGetWindowUserPointer(window);
				auto input_module = ecs->get_mut<mod::Input>();
				if (input_module) {
					comp::inp::KeyState ks = input_module->mouse_buttons[button];
					if (action == GLFW_PRESS) {
						ks.down = true;
						ks.time_pressed = ecs->get_info()->world_time_total;
						ks.pressed = true;
					}
					else if (action == GLFW_RELEASE) {
						ks.down = false;
						ks.released = true;
					}
					input_module->mouse_buttons[button] = ks;
				}
				});

			glfwSetWindowSizeCallback(glfw.window, [](GLFWwindow* window, int width, int height) {
				flecs::world* ecs = (flecs::world*)glfwGetWindowUserPointer(window);
				ecs->module<mod::GLFW>().set<comp::gfx::WindowSize>({ width, height });			
			});


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
			const comp::gfx::GLFW* glfw = e.get<comp::gfx::GLFW>();
			glfwSetWindowSize(glfw->window, size.width, size.height);
		}
	);

	ecs.observer<comp::gfx::WindowTitle>("::observe::gfx::WindowTitle::OnSet")
		.event(flecs::OnSet)
		.each([](flecs::entity e, comp::gfx::WindowTitle& title) {
			spdlog::info("GLFW Setting Window Title to {}", title.title);
			const comp::gfx::GLFW* glfw = e.get<comp::gfx::GLFW>();
			glfwSetWindowTitle(glfw->window, title.title.c_str());
	});

	ecs.observer<comp::gfx::TargetFPS>("::observe::gfx::TargetFPS::OnSet")
		.event(flecs::OnSet)
		.each([](flecs::entity e, comp::gfx::TargetFPS& fps) {
			spdlog::info("Setting target FPS to {}", fps.value);
			e.world().set_target_fps((float)fps.value);
	});

	ecs.system("::sys::glfw::ProcessEvents")
		.kind(flecs::PreFrame)
		.iter([](flecs::iter& it) {
			//spdlog::info(it.system().name().c_str());
			auto input_module = it.world().get_mut<mod::Input>();
			auto glfw_module = it.world().module<mod::GLFW>();
			if (input_module && glfw_module) {
				auto glfw = glfw_module.get_mut<comp::gfx::GLFW>();
				if (glfw->window) {
					for (auto& [key, state] : input_module->mouse_buttons) {
						state.pressed = false;
						state.released = false;
					}
					glm::dvec3 pos;
					glfwPollEvents();
					glfwGetCursorPos(glfw->window, &pos.x, &pos.y);
					input_module->mouse_delta = pos - input_module->mouse_pos;
					input_module->mouse_pos = pos;
				}
			}
	});

	ecs.system("::sys::CheckExit")
		.kind(flecs::PostFrame)
		.iter([](flecs::iter& it) {
		auto glfw = it.world().module<mod::GLFW>().get<comp::gfx::GLFW>();
		if (glfwWindowShouldClose(glfw->window)) {
			it.world().quit();
		}
	});

}
