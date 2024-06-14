#include <flecs.h>
#include <iostream>

using namespace std;

struct CompA { 
	int value = 0; 
	CompA(int value_=999) {
		value = value_;
		//cout << "CompA constructor: " << value << endl;
	}
};
struct CompB {};


int main() {
	flecs::world ecs;

	auto e1 = ecs.entity("e1")
		.add<CompB>()
		//.set<CompA>({ 5 })
		;


	ecs.system()
		.kind(flecs::PreUpdate)
		.write<CompA>()
		.no_readonly()
		.iter([](flecs::iter& it) {
			//it.world().defer_suspend();
			it.world().remove_all<CompA>();			
			//it.world().defer_resume();
			cout << "Removed all CompA" << endl;
			it.world().entity("e1").set<CompA>({ 10 });
		});

	ecs.system()
		.kind(flecs::PostUpdate)
		.iter([](flecs::iter& it) {
		auto a = it.world().entity("e1").get<CompA>();
		cout << "Got CompA " << a->value << endl;
	});

	ecs.progress();
	ecs.progress();

	return 0;
}
