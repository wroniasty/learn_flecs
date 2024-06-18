#include <iostream>
#include <flecs.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

#include <format>
#include <filesystem>

#include "modules/position.h"
#include "modules/scenegraph.h"

#include "Windows.h"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
	flecs::world ecs;
	
	spdlog::info("Working directory: {}", fs::current_path().string());


	ecs.set_target_fps(60);
	ecs.set<flecs::Rest>({});	
	
	ecs.import<mod::Position>();
	ecs.import<mod::SceneGraph>();

	std::string main_file = (fs::current_path() / "plecs" / "main.flecs").string();

	ecs.plecs_from_file(
		main_file.c_str()
	);

	bool exit = false;
	while (!exit) {
		ecs.progress();
		if (GetKeyState(VK_ESCAPE) & 0x8000) {
			exit = true;
		}
		else if (GetKeyState(VK_F8) & 0x8000) {
			auto engine_root = ecs.lookup("Engine");
			if (engine_root.is_valid())
				engine_root.destruct();
			//ecs.lookup("Engine").destruct();
		}
		else if (GetKeyState(VK_F5) & 0x8000) {
			auto engine_root = ecs.lookup("Engine");
			if (engine_root.is_valid())
				engine_root.destruct();
			ecs.plecs_from_file(
				main_file.c_str()
			);
		}
	}

	return 1;
}