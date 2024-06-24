#include "render_sandbox.h"

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <imgui.h>
#include <imgui_bgfx.h>

#include "../input.h"
#include "../gfx/glfw_bridge.h"
#include "render_sandbox.h"
#include "../../resources/shaders/shaders.h"

namespace game {

	void RenderSandboxMode::init(const flecs::world& ecs) 
	{
		Mode::init(ecs);
		spdlog::info("RenderTestInit");
		//m_vbh = bgfx::createVertexBuffer(
		//	// Static data can be passed with bgfx::makeRef
		//	bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
		//	// This layout describes the data in the vertex buffer
		//	Vertex::layout
		//);

		//m_ibh = bgfx::createIndexBuffer(
		//	// Static data can be passed with bgfx::makeRef
		//	bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices))
		//);

		//m_program = loadProgram("vs_cubes", "fs_cubes");
		m_program = bgfx::createProgram(
			bgfx::createShader(bgfx::makeRef(shader::vs_standard, sizeof(shader::vs_standard))),
			bgfx::createShader(bgfx::makeRef(shader::fs_standard, sizeof(shader::fs_standard))),
			true
		);
	}
	void RenderSandboxMode::draw(const flecs::world& ecs)
	{
		input_module = ecs.get_mut<::mod::Input>();
		glfw_module = ecs.module<::mod::GLFW>();

		debug_ui(ecs);

		bgfx::touch(1);
		bgfx::frame();
	}
	void RenderSandboxMode::exit(const flecs::world& ecs)
	{
		Mode::exit(ecs);
		spdlog::info("RenderTestExit");
		//bgfx::destroy(m_vbh);
		//bgfx::destroy(m_ibh);
		bgfx::destroy(m_program);
	}

}