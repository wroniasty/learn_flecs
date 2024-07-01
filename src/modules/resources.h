#pragma once

#include <flecs.h>

#include <string>

namespace res {

	const char* SHADER_PATH = "resources/shaders/";

	struct ShaderResource {

	};
	struct TextureResource {

	};

	struct MeshResource {

	};

	struct ModelResource {
	};

	struct MaterialResource {
		std::string filename;
	};

}

namespace comp::res {


	struct Shader {
		flecs::entity resource;
	};


	struct Texture {
		flecs::entity resource;
	};


	struct Mesh {
		flecs::entity resource;
	};

	struct Model {
		flecs::entity resource;
	};



}

namespace mod {

	struct ShaderLoader {
		ShaderLoader(flecs::world& ecs);
	};

	struct TextureLoader {
		TextureLoader(flecs::world& ecs);
	};

	struct MeshLoader {
		MeshLoader(flecs::world& ecs);
	};

	struct ResourceModule {
		ResourceModule(flecs::world& ecs);
	};
}