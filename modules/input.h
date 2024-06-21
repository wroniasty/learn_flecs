#pragma once

#include <flecs.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <map>
#include <string>
#include <variant>

namespace comp {
	namespace inp {
		struct KeyState {
			bool down;
			float time_pressed;

			bool pressed;
			bool released;

			bool hold(float t, float hold_time) {
				return t - time_pressed >= hold_time;
			}
		};

		struct Stick {
			glm::vec2 value;
		};

		struct Activator {};

	}
}

namespace in = comp::inp;

namespace mod {
	struct Input {
		Input(flecs::world& ecs);

		typedef std::map<std::string, int> KeyMap;
		typedef std::map<int, flecs::entity> KeyEntityMap;
		typedef std::map<int, comp::inp::KeyState> KeyStateMap;
		typedef std::map<std::string, in::Stick> StickMap;
		typedef std::map<int, std::pair<std::string, int>> StickKeyMap;
		typedef std::map<std::string, bool> ActionMap;
		typedef std::map<int, std::string> ActionKeyMap;

		static const int max_keys = 512;

		in::Stick get_stick(std::string name);
		bool get_action(std::string name);

		StickMap sticks;
		StickKeyMap stick_keys;
		ActionMap actions;
		ActionKeyMap action_keys;

		glm::dvec3 mouse_pos;
		glm::dvec3 mouse_delta;
		std::map<int, comp::inp::KeyState> mouse_buttons = {
			{GLFW_MOUSE_BUTTON_LEFT, {}},
			{GLFW_MOUSE_BUTTON_RIGHT, {}},
			{GLFW_MOUSE_BUTTON_MIDDLE, {}},
			{GLFW_MOUSE_BUTTON_4, {}},
			{GLFW_MOUSE_BUTTON_5, {}},
			{GLFW_MOUSE_BUTTON_6, {}},
			{GLFW_MOUSE_BUTTON_7, {}},
			{GLFW_MOUSE_BUTTON_8, {}}
		};

		comp::inp::KeyState key_state_map[max_keys] = {};
		KeyEntityMap key_entity_map;
		KeyMap key_map = {
			KeyMap::value_type("KEY_SPACE", 32),
			KeyMap::value_type("KEY_APOSTROPHE", 39),
			KeyMap::value_type("KEY_COMMA", 44),
			KeyMap::value_type("KEY_MINUS", 45),
			KeyMap::value_type("KEY_PERIOD", 46),
			KeyMap::value_type("KEY_SLASH", 47),
			KeyMap::value_type("KEY_0", 48),
			KeyMap::value_type("KEY_1", 49),
			KeyMap::value_type("KEY_2", 50),
			KeyMap::value_type("KEY_3", 51),
			KeyMap::value_type("KEY_4", 52),
			KeyMap::value_type("KEY_5", 53),
			KeyMap::value_type("KEY_6", 54),
			KeyMap::value_type("KEY_7", 55),
			KeyMap::value_type("KEY_8", 56),
			KeyMap::value_type("KEY_9", 57),
			KeyMap::value_type("KEY_SEMICOLON", 59),
			KeyMap::value_type("KEY_EQUAL", 61),
			KeyMap::value_type("KEY_A", 65),
			KeyMap::value_type("KEY_B", 66),
			KeyMap::value_type("KEY_C", 67),
			KeyMap::value_type("KEY_D", 68),
			KeyMap::value_type("KEY_E", 69),
			KeyMap::value_type("KEY_F", 70),
			KeyMap::value_type("KEY_G", 71),
			KeyMap::value_type("KEY_H", 72),
			KeyMap::value_type("KEY_I", 73),
			KeyMap::value_type("KEY_J", 74),
			KeyMap::value_type("KEY_K", 75),
			KeyMap::value_type("KEY_L", 76),
			KeyMap::value_type("KEY_M", 77),
			KeyMap::value_type("KEY_N", 78),
			KeyMap::value_type("KEY_O", 79),
			KeyMap::value_type("KEY_P", 80),
			KeyMap::value_type("KEY_Q", 81),
			KeyMap::value_type("KEY_R", 82),
			KeyMap::value_type("KEY_S", 83),
			KeyMap::value_type("KEY_T", 84),
			KeyMap::value_type("KEY_U", 85),
			KeyMap::value_type("KEY_V", 86),
			KeyMap::value_type("KEY_W", 87),
			KeyMap::value_type("KEY_X", 88),
			KeyMap::value_type("KEY_Y", 89),
			KeyMap::value_type("KEY_Z", 90),
			KeyMap::value_type("KEY_LEFT_BRACKET", 91),
			KeyMap::value_type("KEY_BACKSLASH", 92),
			KeyMap::value_type("KEY_RIGHT_BRACKET", 93),
			KeyMap::value_type("KEY_GRAVE_ACCENT", 96),
			KeyMap::value_type("KEY_WORLD_1", 161),
			KeyMap::value_type("KEY_WORLD_2", 162),
			KeyMap::value_type("KEY_ESCAPE", 256),
			KeyMap::value_type("KEY_ENTER", 257),
			KeyMap::value_type("KEY_TAB", 258),
			KeyMap::value_type("KEY_BACKSPACE", 259),
			KeyMap::value_type("KEY_INSERT", 260),
			KeyMap::value_type("KEY_DELETE", 261),
			KeyMap::value_type("KEY_RIGHT", 262),
			KeyMap::value_type("KEY_LEFT", 263),
			KeyMap::value_type("KEY_DOWN", 264),
			KeyMap::value_type("KEY_UP", 265),
			KeyMap::value_type("KEY_PAGE_UP", 266),
			KeyMap::value_type("KEY_PAGE_DOWN", 267),
			KeyMap::value_type("KEY_HOME", 268),
			KeyMap::value_type("KEY_END", 269),
			KeyMap::value_type("KEY_CAPS_LOCK", 280),
			KeyMap::value_type("KEY_SCROLL_LOCK", 281),
			KeyMap::value_type("KEY_NUM_LOCK", 282),
			KeyMap::value_type("KEY_PRINT_SCREEN", 283),
			KeyMap::value_type("KEY_PAUSE", 284),
			KeyMap::value_type("KEY_F1", 290),
			KeyMap::value_type("KEY_F2", 291),
			KeyMap::value_type("KEY_F3", 292),
			KeyMap::value_type("KEY_F4", 293),
			KeyMap::value_type("KEY_F5", 294),
			KeyMap::value_type("KEY_F6", 295),
			KeyMap::value_type("KEY_F7", 296),
			KeyMap::value_type("KEY_F8", 297),
			KeyMap::value_type("KEY_F9", 298),
			KeyMap::value_type("KEY_F10", 299),
			KeyMap::value_type("KEY_F11", 300),
			KeyMap::value_type("KEY_F12", 301),
			KeyMap::value_type("KEY_F13", 302),
			KeyMap::value_type("KEY_F14", 303),
			KeyMap::value_type("KEY_F15", 304),
			KeyMap::value_type("KEY_F16", 305),
			KeyMap::value_type("KEY_F17", 306),
			KeyMap::value_type("KEY_F18", 307),
			KeyMap::value_type("KEY_F19", 308),
			KeyMap::value_type("KEY_F20", 309),
			KeyMap::value_type("KEY_F21", 310),
			KeyMap::value_type("KEY_F22", 311),
			KeyMap::value_type("KEY_F23", 312),
			KeyMap::value_type("KEY_F24", 313),
			KeyMap::value_type("KEY_F25", 314),
			KeyMap::value_type("KEY_KP_0", 320),
			KeyMap::value_type("KEY_KP_1", 321),
			KeyMap::value_type("KEY_KP_2", 322),
			KeyMap::value_type("KEY_KP_3", 323),
			KeyMap::value_type("KEY_KP_4", 324),
			KeyMap::value_type("KEY_KP_5", 325),
			KeyMap::value_type("KEY_KP_6", 326),
			KeyMap::value_type("KEY_KP_7", 327),
			KeyMap::value_type("KEY_KP_8", 328),
			KeyMap::value_type("KEY_KP_9", 329),
			KeyMap::value_type("KEY_KP_DECIMAL", 330),
			KeyMap::value_type("KEY_KP_DIVIDE", 331),
			KeyMap::value_type("KEY_KP_MULTIPLY", 332),
			KeyMap::value_type("KEY_KP_SUBTRACT", 333),
			KeyMap::value_type("KEY_KP_ADD", 334),
			KeyMap::value_type("KEY_KP_ENTER", 335),
			KeyMap::value_type("KEY_KP_EQUAL", 336),
			KeyMap::value_type("KEY_LEFT_SHIFT", 340),
			KeyMap::value_type("KEY_LEFT_CONTROL", 341),
			KeyMap::value_type("KEY_LEFT_ALT", 342),
			KeyMap::value_type("KEY_LEFT_SUPER", 343),
			KeyMap::value_type("KEY_RIGHT_SHIFT", 344),
			KeyMap::value_type("KEY_RIGHT_CONTROL", 345),
			KeyMap::value_type("KEY_RIGHT_ALT", 346),
			KeyMap::value_type("KEY_RIGHT_SUPER", 347),
			KeyMap::value_type("KEY_MENU", 348)
		};


	};

}	
