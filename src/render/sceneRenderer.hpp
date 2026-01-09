#pragma once
#include <memory>
#include <vector>

#include "../shader/shader.hpp"
#include "../model/model.hpp"
#include "../camera/camera3D.hpp"

#include "../physics/gameObject.hpp"
#include "renderable.hpp"



class SceneRenderer
{
private:
	std::vector<Model*> models;
	std::vector<GameObject*> gameObjects;
	std::vector<Renderable::Renderable*> renderables;
	std::unique_ptr<Shader> sceneShader;
	std::unique_ptr<Shader> textShader;
	std::unique_ptr<Camera3D> camera;
	int sceneWidth, sceneHeight;
	bool initialized = false;

	void loadModels();
	void addGameObject(GameObject* _gameObject);
	
	void initCamera();
	void initSceneShader();
	void initProjectionMatrix();
	void initViewMatrix();
	void drawRenderables();
	void populateRenderables();
	void useSceneShader();
	void updateRenderables(float _deltaTime);
	// adds a new renderable to the list of renderables for each position in the list of positions given, and the model will be whatever model ID is passed.
	void addRenderables(std::vector<glm::vec3> _pos, unsigned int _id);

public:
	SceneRenderer(const unsigned int _screenWidth, const unsigned int _screenHeight);
	~SceneRenderer() = default;

	void initSceneRenderer();
	void RenderScene(float _deltaTime);

	Camera3D& getCamera();
	bool hasBeenInitialized() const;
};

