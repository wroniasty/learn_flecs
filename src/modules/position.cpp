#include "position.h"

using namespace comp;


mod::Position::Position(flecs::world& ecs)
{
	ecs.component<comp::Vec3d>("::c::Vec3d")
		.member<comp::Vec3d::value_type>("x")
		.member<comp::Vec3d::value_type>("y")
		.member<comp::Vec3d::value_type>("z");

	ecs.component<comp::Vec3f>("::c::Vec3f")
		.member<comp::Vec3f::value_type>("x")
		.member<comp::Vec3f::value_type>("y")
		.member<comp::Vec3f::value_type>("z");

	ecs.component<glm::ivec2>("::c::Vec2i")
		.member<glm::ivec2::value_type>("x")
		.member<glm::ivec2::value_type>("y");

	ecs.component<glm::vec2>("::c::Vec2f")
		.member<glm::vec2::value_type>("x")
		.member<glm::vec2::value_type>("y");

	ecs.component<comp::Local>("::c::Local");
	ecs.component<comp::World>("::c::World");
	ecs.component<comp::Rotation>("::c::Rotation");

}
