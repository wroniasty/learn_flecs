#include <iostream>
#include <flecs.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

#include <format>
#include "modules/position.h"
#include "modules/scenegraph.h"

#include "Windows.h"


int main(int argc, char** argv) {
	flecs::world ecs;
	
	ecs.set_target_fps(60);
	ecs.set<flecs::Rest>({});
	
	ecs.import<mod::Position>();
	ecs.import<mod::SceneGraph>();

	bool exit = false;
	while (!exit) {
		ecs.progress();
		if (GetKeyState(VK_ESCAPE) & 0x8000) {
			exit = true;
		}
	}

	return 1;
}