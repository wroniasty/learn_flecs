#include "scenegraph.h"
#include "position.h"

#include <spdlog/spdlog.h>

using namespace comp;

/*
	Update the world position of nodes in the scene graph
*/
void _system_update_tree_world_position(flecs::iter& it, const Vec3d *local, Vec3d *world, Vec3d *parent) {	
	for (auto i : it) {
		auto e = it.entity(i);
		world[i] = local[i];
		if (parent) {
			world[i] += parent[i];
		}
	}
} 

flecs::system sys::Register_UpdateTreeWorldPosition(flecs::world& ecs) {
	return ecs.system<const Vec3d, Vec3d, Vec3d>("::sys::SceneGraph::UpdateSceneTreeWorldPosition")
		.with<Node>()
		.without(flecs::Disabled).up(flecs::ChildOf)   // Only update entities that do not have a disabled ancestor
		.term_at(1).second<Local>()
		.term_at(2).second<World>().inout()
		.term_at(3).second<World>()
		.term_at(3).parent().cascade().optional()
		.kind(flecs::PostUpdate)				
		.iter(_system_update_tree_world_position);
}

/*
	Offset the scene position of nodes in the scene graph
*/
void _system_offset_scene_position(flecs::iter& it, const Vec3d *world, Vec3d *scene, SceneRoot *sr, SceneRoot *sr_own) {
	const Vec3d *offset;
	flecs::entity origin_entity;
	if (sr) {
		origin_entity = sr->scene_origin;
	}
	else {
		origin_entity = sr_own->scene_origin;
	}
	if (origin_entity.is_valid()) {
		offset = origin_entity.get<Vec3d, World>();
		if (offset) {
			for (auto i : it) {
				scene[i] = world[i] - (*offset);
			}
		}
	}
}

flecs::system sys::Register_OffsetScenePosition(flecs::world& ecs) {
		return ecs.system<const Vec3d, Vec3d, SceneRoot, SceneRoot>("::sys::SceneGraph::OffsetScenePosition")
		.with<Node>()
		.without(flecs::Disabled).up(flecs::ChildOf)   // Only update entities that do not have a disabled ancestor
		.term_at(1).second<World>()
		.term_at(2).second<Scene>()
		.term_at(3).parent().cascade().oper(flecs::Or)
		.term_at(4)
		.kind(flecs::PreStore)
		.iter(_system_offset_scene_position);
}

mod::SceneGraph::SceneGraph(flecs::world& ecs) {
	spdlog::info("Importing mod::SceneGraph");
	
	ecs.import<mod::Position>();
	ecs.component<Node>("::c::Node");	
	ecs.component<Scene>("::c::Scene");
	
	ecs.component<SceneRoot>("::c::SceneRoot")
		.member<flecs::entity_t>("scene_origin")
		;

	update_tree_world_position = sys::Register_UpdateTreeWorldPosition(ecs);
	offset_scene_position = sys::Register_OffsetScenePosition(ecs);
	spdlog::info("UpdateTreeWorldSystem: query={}", update_tree_world_position.query().str().c_str());	
	spdlog::info("OffsetScenePosition: query={}", offset_scene_position.query().str().c_str());
}

void mod::SceneGraph::setSceneRoot(flecs::world &ecs, flecs::entity_t e, flecs::entity_t origin) const
{	
	ecs.filter_builder<SceneRoot>()
		.term<SceneRoot>()
		.each([e](flecs::entity e, SceneRoot& scene) {
			e.remove<SceneRoot>();
		});
	if (origin == 0) {
		origin = e;
	}
	ecs.entity(e).set<SceneRoot>({ ecs.entity(origin) } );
}

void mod::SceneGraph::setSceneOrigin(flecs::world& ecs, flecs::entity_t origin) const
{
	ecs.filter_builder<SceneRoot>()
		.term<SceneRoot>()
		.each([origin, &ecs](flecs::entity e, SceneRoot& scene) {
			scene.scene_origin = ecs.entity(origin);
			e.set<SceneRoot>(scene);
	});
}

void mod::SceneGraph::setSceneOriginDefer(flecs::world& ecs, flecs::entity_t origin) const
{
	ecs.defer_begin();
	setSceneOrigin(ecs, origin);
	ecs.defer_end();
}

void mod::SceneGraph::setSceneRootDefer(flecs::world& ecs, flecs::entity_t e, flecs::entity_t origin) const
{
	ecs.defer_begin();
	setSceneRoot(ecs, e, origin);
	ecs.defer_end();
}


#ifdef _TEST_SUITE

#include <catch2/catch_test_macros.hpp>
#include <functional>

void _iter_tree(flecs::entity e, int depth, std::function<bool(flecs::entity, int)> f) {
	if (!f(e, depth)) {
		return;
	}
	e.children([&](flecs::entity child) {
		_iter_tree(child, depth + 1, f);
	});
}

TEST_CASE("Scene Graph Module") {
	flecs::world ecs;

	ecs.import<mod::SceneGraph>();		
	auto _prefab = ecs.prefab("Node")
		.override<Position, Local>()
		.override<Position, World>()
		.override<Position, Scene>()
		.add<Node>();

	ecs.observer<SceneRoot>("SceneRootObserver_Set")
		.event(flecs::OnSet)
		.each([](flecs::entity e, SceneRoot& scene) {
			spdlog::info("SceneRoot is set on: {} with origin: {}", e.path().c_str(), scene.scene_origin);
		});

	ecs.observer<SceneRoot>("SceneRootObserver_Remove")
		.event(flecs::OnSet)
		.each([](flecs::entity e, SceneRoot& scene) {
			spdlog::info("SceneRoot is removed from: {}", e.path().c_str());
		});

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

	SECTION("SceneRoot set") {
		auto root = ecs.entity().is_a(_prefab);
		auto sg = ecs.get<mod::SceneGraph>();
		
		std::vector<flecs::entity> top_level_nodes;
		ecs.each([&top_level_nodes](flecs::entity e, const Node& node) {
			if (e.depth(flecs::ChildOf) == 0) {
				top_level_nodes.push_back(e);
			}
		});

		for (auto root : top_level_nodes) {
			sg->setSceneRootDefer(ecs, root);
			ecs.progress();
			ecs.each([](flecs::entity e, const SceneRoot& scene) {
				spdlog::debug("SceneRoot: {}  origin: {}", e.path().c_str(), scene.scene_origin);
				REQUIRE(scene.scene_origin == e);
			});
			std::vector<flecs::entity> children;
			root.children([&children](flecs::entity child) {
				children.push_back(child);
			});
			for (auto c : children) {
				sg->setSceneOriginDefer(ecs, c);
				ecs.progress();
				_iter_tree(root, 0, [&ecs, &root](flecs::entity e, int depth) {
					auto scene = e.get<Position, Scene>();
					auto world = e.get<Position, World>();
					auto local = e.get<Position, Local>();
					auto sr = root.get<SceneRoot>();
					auto sre = ecs.entity(sr->scene_origin);
					auto origin = sre.get<Position, World>();
					spdlog::info("(SR) Entity: {} scene: {},{}  world: {},{}  origin: {},{} ({})", e.path().c_str(),
						scene->x, scene->y,
						world->x, world->y,
						origin->x, origin->y, sre.path().c_str()
					);
					REQUIRE(scene->x == world->x - origin->x);
					REQUIRE(scene->y == world->y - origin->y);
					return true;
				});
			}
		}


				
	}

};

#endif