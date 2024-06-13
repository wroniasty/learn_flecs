#include <iostream>
#include <flecs.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <Windows.h>
#include <format>
#include "modules/position.h"
#include "modules/scenegraph.h"

using namespace comp;

flecs::entity create_tree(int siblings, int depth, flecs::world& ecs) {
	static int id = 0;
	flecs::entity e = ecs.entity(std::format("node{:04}", id++).c_str())
		.add<Position, Local>()
		.add<Position, World>()
		.add<Node>();
	if (depth > 0) {
		for (int i = 0; i < siblings; i++) {
			auto sibling = create_tree(siblings, depth - 1, ecs);
			sibling.child_of(e);
		}
	}
	return e;
}

struct CurrentFrame {
	int value;
};

void setup_scene_root_remove_add_test(flecs::world& ecs) {
	auto root = create_tree(5, 5, ecs);
	std::vector<flecs::entity> children;
	root.children([&children](flecs::entity e) {
		children.push_back(e);
	});

	ecs.system("RemoveAddTest")
		.kind(flecs::PreUpdate)
		.no_readonly()
		.iter([=](flecs::iter& it) {
			auto cf = it.world().get<CurrentFrame>();
			if (cf->value % 10 == 0) {
				spdlog::info("[{}] SwitchRoot", cf->value);
				auto root_idx = (cf->value / 10) % children.size();
				if (root_idx == 1) root_idx = 0;

				it.world().remove_all<SceneRoot>();
				auto new_root = children[root_idx];
				new_root.set<SceneRoot>({ new_root });
			}
		});

	ecs.observer<SceneRoot>("SceneRootNode")
		.event(flecs::OnSet)
		.iter([](flecs::iter& it) {
			auto cf = it.world().get<CurrentFrame>();
			for (auto i : it) {
				auto sr = it.entity(i).get<SceneRoot>();
				spdlog::info("[{}] Set SceneRoot: {} {}", cf->value, it.entity(i).name().c_str(), sr->scene_origin);
			}
	});

	ecs.observer<SceneRoot>("RemoveRootNode")
		.event(flecs::OnRemove)
		.iter([](flecs::iter& it) {
			auto cf = it.world().get<CurrentFrame>();
			for (auto i : it) {
				spdlog::info("[{}] Remove SceneRoot: {}", cf->value, it.entity(i).name().c_str());
			}
	});

	ecs.system<Node, SceneRoot, SceneRoot>("SceneRootTest")
		.kind(flecs::PostUpdate)
		.iter([](flecs::iter& it) {
			auto cf = it.world().get<CurrentFrame>();
			spdlog::info("[{}] SceneRootTest", cf->value);

			for (auto i : it) {
				
			}
		});

}

int main() {
	flecs::world ecs;
	
	Position::Register(ecs);

	ecs.set<flecs::Rest>({});
	ecs.import<flecs::monitor>();

	ecs.set<CurrentFrame>({ 0 });
	ecs.system<CurrentFrame>("FrameCounter")
		.term_at(1).singleton()
		.kind(flecs::PreFrame)
		.iter([](flecs::iter &it, CurrentFrame *frame) {
			frame->value++;
		});	

	setup_scene_root_remove_add_test(ecs);
	ecs.set_target_fps(5);

	while (true) {
		//ecs.set<int, flecs::>({ frame });
		ecs.progress();		
		if (GetKeyState(VK_ESCAPE) & 0x80) {
			break;
		}
	}

	return 0;
}