#include "render_sandbox.h"

#include <bx/bx.h>
#include <bx/timer.h>
#include <bx/math.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/gtx/transform.hpp>

#include <imgui.h>
#include <imgui_bgfx.h>

#include "../input.h"
#include "../gfx/glfw_bridge.h"
#include "render_sandbox.h"
#include "../../resources/shaders/shaders.h"

namespace game {

	struct PosColorVertex {
		float m_x;
		float m_y;
		float m_z;
		uint32_t m_abgr;

		static void init() {
			ms_layout
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
				.end();
		}

		static bgfx::VertexLayout ms_layout;
	};
	bgfx::VertexLayout PosColorVertex::ms_layout;

	static PosColorVertex s_cubeVertices[] =
	{
		{-1.0f,  1.0f,  1.0f, 0xffff0000 },
		{ 1.0f,  1.0f,  1.0f, 0xffff00ff },
		{-1.0f, -1.0f,  1.0f, 0xffff0fff },
		{ 1.0f, -1.0f,  1.0f, 0xffffffff },
		{-1.0f,  1.0f, -1.0f, 0xffff0000 },
		{ 1.0f,  1.0f, -1.0f, 0xffff0f0f },
		{-1.0f, -1.0f, -1.0f, 0xfff00f0f },
		{ 1.0f, -1.0f, -1.0f, 0xffffffff },
	};


	static const uint16_t s_cubeTriList[] =
	{
		0, 1, 2, // 0
		1, 3, 2,
		4, 6, 5, // 2
		5, 6, 7,
		0, 2, 4, // 4
		4, 2, 6,
		1, 5, 3, // 6
		5, 7, 3,
		0, 4, 1, // 8
		4, 5, 1,
		2, 3, 6, // 10
		6, 3, 7,
	};

	void RenderSandboxMode::init(const flecs::world& ecs) 
	{
		Mode::init(ecs);
		spdlog::info("RenderTestInit");
		PosColorVertex::init();
		m_vbh = bgfx::createVertexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
			// This layout describes the data in the vertex buffer
			PosColorVertex::ms_layout
		);

		m_ibh = bgfx::createIndexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
		);

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
		auto window_size = glfw_module.get<comp::gfx::WindowSize>();

		debug_ui(ecs);

		float time = (float)((bx::getHPCounter()) / double(bx::getHPFrequency()));

		const bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
		const bx::Vec3 eye = { 0.0f, 0.0f, -35.0f };
		{
			glm::mat4x4 view;
			// float view[16];
			// glm::ortho(0.0f, (float)window_size->width, (float)window_size->height, 0.0f, 0.0f, 1000.0f);
			bx::mtxLookAt( (float *) &view, eye, at);

			float proj[16];
			float wh = (float)window_size->width / 2;
			float hh = (float)window_size->height / 2;
//			bx::mtxProj(proj, 120.0f, /*window_size->width / window_size->height*/1.0f, 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
			bx::mtxOrtho(proj, -wh, wh, -hh, hh, 0.0f, 1000.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
			bgfx::setViewTransform(0, &view, proj);

			// Set view 0 default viewport.
			bgfx::setViewRect(0, 0, 0, uint16_t(window_size->width), uint16_t(window_size->height));
		}
		uint64_t state = 0
			| BGFX_STATE_WRITE_R
			| BGFX_STATE_WRITE_G			
			| BGFX_STATE_WRITE_B 
			| BGFX_STATE_WRITE_A 
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			| BGFX_STATE_MSAA
			// | BGFX_STATE_PT_TRISTRIP
			;
		{
			glm::mat4 transform { 1 };
			transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
			transform = glm::rotate(transform, time, glm::vec3(0, 0, time/3));
			transform = glm::scale(transform, glm::vec3(10.0f, 10.0f, 10.0f));

			// Set model matrix for rendering.
			bgfx::setTransform(&transform);

			// Set vertex and index buffer.
			bgfx::setVertexBuffer(0, m_vbh);
			bgfx::setIndexBuffer(m_ibh);

			// Set render states.
			bgfx::setState(state); 

			// Submit primitive for rendering to view 0.
			bgfx::submit(0, m_program);
		}

		bgfx::touch(0);
		bgfx::frame();
	}
	void RenderSandboxMode::exit(const flecs::world& ecs)
	{
		Mode::exit(ecs);
		spdlog::info("RenderTestExit");
		bgfx::destroy(m_vbh);
		bgfx::destroy(m_ibh);
		bgfx::destroy(m_program);
	}

}