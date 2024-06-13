#include "position.h"


void comp::Position::Register(flecs::world& ecs) {
	ecs.component<Position>()
		.member<Position::value_type>("x")
		.member<Position::value_type>("y")
		.member<Position::value_type>("z");
}
