#include "scenegraph.h"
#include "position.h"

#include <spdlog/spdlog.h>

using namespace comp;

void _system_update_tree_world_position(flecs::iter& it, const Position *local, Position *world, Position *parent) {	
	for (auto i : it) {
		auto e = it.entity(i);
		world[i] = local[i];
		if (parent) {
			world[i] += parent[i];
		}
	}
}

flecs::system sys::Register_UpdateTreeWorldPosition(flecs::world& ecs) {
	return ecs.system<const Position, Position, Position>("UpdateSceneTreeWorldPosition")
		.with<Node>()
		.without(flecs::Disabled).up(flecs::ChildOf)   // Only update entities that do not have a disabled ancestor
		.term_at(1).second<Local>()
		.term_at(2).second<World>().inout()
		.term_at(3).second<World>()
		.term_at(3).parent().cascade().optional()
		.kind(flecs::PostUpdate)		
		.iter(_system_update_tree_world_position);
}

mod::SceneGraph::SceneGraph(flecs::world& ecs) {
	spdlog::info("Importing mod::SceneGraph");
	ecs.import<mod::Position>();
	auto system = sys::Register_UpdateTreeWorldPosition(ecs);
	spdlog::info("UpdateTreeWorldSystem: {}", system.query().str().c_str());
	
}

#ifdef _TEST_SUITE

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Scene Graph Module") {
	flecs::world ecs;

	ecs.import<mod::SceneGraph>();		
	auto _prefab = ecs.prefab("Node")
		.override<Position, Local>()
		.override<Position, World>()
		.add<Node>();

	Position default_pos = { 1.0, 1.0, 0.0 };
	for (int i = 0; i < 2; i++) {
		auto ei = ecs.entity().is_a(_prefab);
		for (int j = 0; j < 2; j++) {
			auto ej = ecs.entity().is_a(_prefab);
			ej.set<Position, Local>(default_pos);
			ej.child_of(ei);
			for (int k = 0; k < 2; k++) {
				auto ek = ecs.entity().is_a(_prefab);
				ek.set<Position, Local>(default_pos);
				ek.set<float>(1.0f);
				ek.child_of(ej);
			}
		}
	}



	ecs.progress();

	SECTION("Propagate World Position") {
		ecs.each([](flecs::entity e, const Node& node) {
			auto wpos = e.get<Position, World>();
			auto lpos = e.get<Position, Local>();
			REQUIRE(((int)wpos->x == e.depth(flecs::ChildOf)));
			REQUIRE(((int)wpos->y == e.depth(flecs::ChildOf)));
			});
	}

	SECTION("Subtree disable") {
		ecs.defer_begin();
		ecs.each([](flecs::entity e, const Node& node) {
			if (e.depth(flecs::ChildOf) == 0) {
				e.set<Position, Local>({ 10.0, 10.0, 0.0 });
			} else if (e.depth(flecs::ChildOf) == 1) {
				e.disable();
			}
		});
		ecs.defer_end();
		spdlog::info("Subtree level 1 is disabled");
		ecs.progress();
		ecs.each([](flecs::entity e, const Node& node) {
			auto depth = e.depth(flecs::ChildOf);
			if (depth > 1) {
				REQUIRE(e.get<Position, World>()->x == depth);
				REQUIRE(e.get<Position, World>()->y == depth);
			}
		});
		ecs.defer_begin();
		ecs.filter_builder<Node>()
			.with(flecs::Disabled)
			.each([](flecs::entity e, const Node& node) {			
				e.enable();			
			}
		);
		ecs.defer_end();
		spdlog::info("Subtree level 1 is re-enabled");
		ecs.progress();
		ecs.each([](flecs::entity e, const Node& node) {
			auto depth = e.depth(flecs::ChildOf);			
			auto lpos = e.get<Position, World>();
			if (depth > 0) {
				REQUIRE(lpos->x == depth + 10.);
				REQUIRE(lpos->y == depth + 10.);
			}
		});

	}

};

#endif