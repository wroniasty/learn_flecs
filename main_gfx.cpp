#include <iostream>
#include <flecs.h>

#include <imgui.h>
#include <imgui_bgfx.h>

#include <bx/timer.h>


#include "modules/gfx/glfw_bridge.h"
#include "modules/mode/mode.h"

#include "resources/shaders/shaders.h"

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

struct TestRenderSpeed {
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    bgfx::ProgramHandle m_program;

	TestRenderSpeed() {
		PosColorVertex::init();
		m_vbh = bgfx::createVertexBuffer(
			bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices))
			, PosColorVertex::ms_layout
		);

		m_ibh = bgfx::createIndexBuffer(
			bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
		);
		m_program = bgfx::createProgram(
			bgfx::createShader(bgfx::makeRef(shader::vs_standard, sizeof(shader::vs_standard))),
			bgfx::createShader(bgfx::makeRef(shader::fs_standard, sizeof(shader::fs_standard))),
			true);
	}

};


int main(int, char**)
{
    flecs::world ecs;
    ecs.set<flecs::Rest>({}); // enable rest api
    ecs.set_target_fps(60);
    ecs.import<mod::ModeModule>();

    auto glfw_module = ecs.import<mod::GLFW>()
        .set<comp::gfx::GLFW>({})
        .set<comp::gfx::WindowSize>({ 1000, 600 })
        .set<comp::gfx::WindowTitle>({ "GLFW"})
        .set<comp::gfx::TargetFPS>({ 25 })
        ;

    auto glfw = glfw_module.get<comp::gfx::GLFW>();
    auto mode_controller = ecs.get<mod::ModeModule>();    
    mode_controller->setActiveModule(ecs, "RenderSandbox");
    long frame = 0;
    
    while (ecs.progress()) {
    }

    glfw_module.remove<comp::gfx::GLFW>();    
    

}