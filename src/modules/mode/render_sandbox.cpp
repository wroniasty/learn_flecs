#include "render_sandbox.h"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/gtx/transform.hpp>

#include <imgui.h>

#include "../input.h"
#include "../gfx/glfw_bridge.h"
#include "render_sandbox.h"


namespace game {

	Camera::Camera() : eye(0, 0, -30.f), at(0, 0, 0), up(0, 1, 0) {}
	glm::mat4x4 Camera::getViewMatrix() {

		glm::mat4x4 view = glm::lookAtLH(eye, at, up);
		return view;
	}

	glm::mat4x4 Camera::getOrthoProjection() {
		glm::mat4x4 proj = glm::orthoLH(viewport[0], viewport[1], viewport[2], viewport[3], 0.f, 1000.f);
		return proj;
	}


	void RenderSandboxMode::init(const flecs::world& ecs) 
	{
		Mode::init(ecs);
		spdlog::info("RenderTestInit");
	}
	void RenderSandboxMode::draw(const flecs::world& ecs)
	{
		input_module = ecs.get_mut<::mod::Input>();
		glfw_module = ecs.module<::mod::GLFW>();
		auto window_size = glfw_module.get<comp::gfx::WindowSize>();


	}
	void RenderSandboxMode::exit(const flecs::world& ecs)
	{
		Mode::exit(ecs);
		spdlog::info("RenderTestExit");
	}

}