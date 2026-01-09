#pragma once

namespace Renderable 
{
	struct Renderable {
		Model* model;
		glm::mat4 transform;
		glm::vec3 position;
	};
}