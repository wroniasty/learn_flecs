#include "input.h"

mod::Input::Input(flecs::world& ecs)
{

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
		auto key_state = ecs.entity(kv.first.c_str())
			.set_second<comp::inp::KeyState>(key, {})
			.child_of(state);
	}	

}
