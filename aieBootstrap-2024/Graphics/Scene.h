#pragma once
#include <glm/glm.hpp>
#include <list>
#include <vector>
#include "Lights.h"

class BaseCamera;
class Instance;

const int MAX_LIGHTS = 4;

class Scene
{
public:
	Scene(BaseCamera* _camera, glm::vec2 _windowSize, Lights& _globalLight);
	~Scene();

	void AddInstance(Instance* _instance);
	void Update(float _dt);
	void Draw();
	void ImGUI_Functions();

	void AddPointLights(Lights _light) { m_pointLights.push_back(_light); };

	BaseCamera* GetCamera() { return m_camera; }
	glm::vec2 GetWindowSize() { return m_windowSize; }
	glm::vec3 GetAmbeintLightColor() { return m_globalLight.color; }
	void SetAmbientLightColor(glm::vec3 _color) { m_globalLight.color = _color; }

	Lights& GetGlobalLight() { return m_globalLight; }

	std::vector<Lights>& GetPointLights() { return m_pointLights; }
	int GetNumberOfLights() { return m_pointLights.size(); }

	glm::vec3* GetPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* GetPointLightColors() { return &m_pointLightColors[0]; }

protected:

	bool m_addLight;

	BaseCamera* m_camera;
	glm::vec2 m_windowSize;
	std::list<Instance*> m_instances;
	std::vector<Lights> m_pointLights;
	Lights m_globalLight;


	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColors[MAX_LIGHTS];
};

