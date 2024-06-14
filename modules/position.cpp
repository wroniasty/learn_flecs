#include "position.h"

using namespace comp;


mod::Position::Position(flecs::world& ecs)
{
	ecs.component<comp::Position>("Position")
		.member<comp::Position::value_type>("x")
		.member<comp::Position::value_type>("y")
		.member<comp::Position::value_type>("z");
}
