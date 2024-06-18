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

	ecs.component<comp::Local>("::c::Local");
	ecs.component<comp::World>("::c::World");
	ecs.component<comp::Rotation>("::c::Rotation");

}
