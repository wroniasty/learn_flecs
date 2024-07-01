#pragma once

#include "mode.h"

namespace game {
	
	struct Camera {
		glm::vec3 eye;
		glm::vec3 at;
		glm::vec3 up;

		glm::vec4 viewport;
		float zoom = 1.0f;

		Camera();

		glm::mat4x4 getViewMatrix();
		glm::mat4x4 getProjectionMatrix(float aspect);
		glm::mat4x4 getOrthoProjection();
		glm::mat4x4 getPerspectiveProjection();
	};

	struct RenderSandboxMode : public Mode {

		Camera m_camera;

		void init(const flecs::world& ecs) override;
		void draw(const flecs::world& ecs) override;
		void exit(const flecs::world& ecs) override;
	};

}