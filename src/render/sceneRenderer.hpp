#pragma once
#include <memory>
#include <vector>

#include "../shader/shader.hpp"
#include "../model/model.hpp"
#include "../camera/camera3D.hpp"

struct Renderable {
	Model* model;
	glm::mat4 transform;
};

class SceneRenderer
{
private:
	std::vector<std::unique_ptr<Model>> models;
	std::vector<Renderable> renderables;
	std::unique_ptr<Shader> sceneShader;
	std::unique_ptr<Shader> textShader;
	std::unique_ptr<Camera3D> camera;
	int sceneWidth, sceneHeight;
	bool initialized = false;

	void loadModels();
	
	void initCamera();
	void initSceneShader();
	void initProjectionMatrix();
	void initViewMatrix();
	void drawRenderables();
	void populateRenderables();
	void useSceneShader();

public:
	SceneRenderer(const unsigned int _screenWidth, const unsigned int _screenHeight);
	~SceneRenderer() = default;

	void initSceneRenderer();
	void RenderScene();

	Camera3D& getCamera();
	bool hasBeenInitialized() const;
};

