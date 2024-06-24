#include "input.h"
#include <spdlog/spdlog.h>

#include <filesystem>

namespace fs = std::filesystem;

mod::Input::Input(flecs::world& ecs)
{
	ecs.component<comp::inp::Stick>("::c::inp::Stick")
		.member<glm::vec2>("value")
		;
	ecs.component<comp::inp::Activator>("::c::inp::Activator");

	auto key_state_component = ecs.component<comp::inp::KeyState>("::c::inp::KeyState")
		.member<bool>("down")
		.member<float>("time_pressed")
		;

	auto input = ecs.entity("::Input");
	auto keyboard = ecs.entity("Keyboard")
		.child_of(input);
	auto state = ecs.entity("State")
		.child_of(input);

	for (auto kv : key_map) {
		auto key = ecs.entity(kv.first.c_str())
			.child_of(keyboard);
		key_entity_map[kv.second] = key;
		key_state_map[kv.second] = comp::inp::KeyState{};
	}	

	std::string mapping_file = (fs::current_path() / "plecs" / "input_mapping.flecs").string();
	if (ecs.plecs_from_file(mapping_file.c_str()) == -1) {
		spdlog::error("Failed to load input mapping from file {}", mapping_file);
	}
	else {
		flecs::entity input_config = ecs.lookup("::Engine::InputConfig");
		// All input mappings are children of the InputConfig entity
		input_config.children([&](flecs::entity input_def) {
			auto mapping = input_def.lookup("Mapping");
			if (input_def.has<comp::inp::Stick>()) {
				std::vector<std::string> directions = { "Up", "Down", "Left", "Right" };
				int direction_idx = 1;
				for (auto dir : directions) {
					if (auto keys = mapping.lookup(dir.c_str())) {
						keys.children([&](flecs::entity key) {
							spdlog::info("{}:{} = {}", input_def.name().c_str(), dir, key.name().c_str());
							auto key_value = this->key_map.at(key.name().c_str());
							this->stick_keys[key_value] = { input_def.name().c_str(), direction_idx};
						});
					}
					direction_idx++;
				}
				this->sticks[input_def.name().c_str()] = comp::inp::Stick{ glm::vec2(0.0f) };
			} else if (input_def.has<comp::inp::Activator>()) {
				if (auto exec = mapping.lookup("Exec")) {
					exec.children([&](flecs::entity key) {
						spdlog::info("{} = {}", input_def.name().c_str(), key.name().c_str());
						auto key_value = this->key_map.at(key.name().c_str());
						this->action_keys[key_value] = input_def.name().c_str();
					});
					this->actions[input_def.name().c_str()] = { 0 };
				}
			}
		});
	}

	ecs.system("::sys::UpdateInputActions")
		.kind(flecs::OnLoad)
		.iter([](flecs::iter& it) {
			auto input_module = it.world().get_mut<mod::Input>();
			for (auto &kv : input_module->action_keys) {
				input_module->actions[kv.second] = input_module->key_state_map[kv.first];
			}
			for (auto &kv : input_module->sticks) {
				auto stick_name = kv.first;
				kv.second.value = glm::vec2(0.0f);
			}
			for (auto kv : input_module->stick_keys) {
				auto key = kv.first;
				if (input_module->key_state_map[key].down) {
					auto [stick_name, direction] = kv.second;
					glm::ivec2 direction_vector;
					switch (direction) {
						case 1:
							direction_vector = { 0, -1 }; break;
						case 2:
							direction_vector = { 0, 1 }; break;
						case 3:
							direction_vector = { -1, 0 }; break;
						case 4:
							direction_vector = { 1, 0 }; break;
					}
					auto it = input_module->sticks.find(stick_name);
					if (it != input_module->sticks.end()) {
						it->second.value += direction_vector;						
					}
				}
			}
		}
	);

	ecs.system("::sys::ClearInputKeyboardStates")
		.kind(flecs::PostFrame)
		.iter([](flecs::iter& it) {
			//spdlog::info(it.system().name().c_str());
			auto input_module = it.world().get_mut<mod::Input>();
			for (int i = 0; i < input_module->max_keys; i++) {
				if (input_module->key_state_map[i].pressed) {
					spdlog::info("Key {} ({}) was pressed", input_module->key_entity_map.at(i).name().c_str(), i);
				}
				if (input_module->key_state_map[i].released) {
					spdlog::info("Key {} ({}) was released", input_module->key_entity_map.at(i).name().c_str(), i);
				}
				input_module->key_state_map[i].pressed = false;
				input_module->key_state_map[i].released = false;
			}
		}
	);

}

in::Stick mod::Input::get_stick(std::string name)
{
	auto it = sticks.find(name);
	if (it != sticks.end()) {
		return it->second;
	}
	else {
		spdlog::error("Stick {} not found", name);
	}
	return in::Stick();
}

bool mod::Input::get_action(std::string name)
{
	auto it = actions.find(name);
	if (it != actions.end()) {
		return it->second.down;
	}
	else {
		spdlog::error("Action {} not found", name);
	}
	return false;
}
