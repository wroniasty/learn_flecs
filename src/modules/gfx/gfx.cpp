#include "gfx.h"

mod::Gfx::Gfx(flecs::world& ecs)
{
	ecs.component<comp::gfx::WindowSize>("::c::gfx::WindowSize")
		.member<int>("width")
		.member<int>("height")
		;
	ecs.component<comp::gfx::WindowTitle>("::c::gfx::WindowTitle")
		.member<std::string>("title")
		;
}
