#include <iostream>
#include <flecs.h>
//#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

struct Node {};
struct World {};
struct Local {};

struct Position : public glm::dvec3 {};

struct SceneRoot {
	flecs::entity_t origin;
};



int main() {
	int frames = 10;
	flecs::world ecs;

	ecs.component<Position>()
		.member<Position::value_type>("x")
		.member<Position::value_type>("y")
		.member<Position::value_type>("z");


	ecs.set_target_fps(10);

	while (frames-- > 0) {
		ecs.progress();
		//spdlog::info("Frame {0}", frames);
	}

	return 1;
}