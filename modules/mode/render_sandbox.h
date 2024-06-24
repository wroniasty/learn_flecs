#pragma once
#include <bgfx/bgfx.h>

#include "mode.h"

namespace game {
	
	struct RenderSandboxMode : public Mode {
		bgfx::VertexBufferHandle m_vbh;
		bgfx::IndexBufferHandle m_ibh;
		bgfx::ProgramHandle m_program;

		void init(const flecs::world& ecs) override;
		void draw(const flecs::world& ecs) override;
		void exit(const flecs::world& ecs) override;
	};

}