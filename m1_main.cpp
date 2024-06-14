#include <flecs.h>
#include <iostream>

using namespace std;

struct CompA { 
	int value = 999; 
	CompA(int value_=998) {
		value = value_;
		cout << "CompA constructor: " << value << endl;
	}
};
struct CompB {};


int main() {
	flecs::world ecs;

	auto e1 = ecs.entity("e1")
		.add<CompB>()
		.set<CompA>({ 5 })
		;


	ecs.system()
		.kind(flecs::PreUpdate)
		.write<CompA>()
		.no_readonly()
		.iter([](flecs::iter& it) {
			it.world().defer_suspend();
			it.world().remove_all<CompA>();			
			it.world().defer_resume();
			cout << "Removed all CompA" << endl;
			it.world().entity("e1").set<CompA>({ 10 });
		});

	ecs.system()
		.kind(flecs::PostUpdate)
		.iter([](flecs::iter& it) {
		auto a = it.world().entity("e1").get<CompA>();
		cout << "Got CompA " << a->value << endl;
	});

	ecs.observer<CompA>()
		.event(flecs::OnSet)
		.iter([](flecs::iter& it) {
		for (auto i : it) {
				auto a = it.entity(i).get<CompA>();
				cout << "Set CompA " << a->value << endl;
			}
		});

	ecs.observer<CompA>()
		.event(flecs::OnRemove)
		.iter([](flecs::iter& it) {
		for (auto i : it) {
				cout << "Removed CompA" << endl;
			}
		});

	ecs.progress();
	ecs.progress();

	return 0;
}
