#pragma once
#include <glm/glm.hpp>
#include <list>
#include <vector>
#include <string>
#include "Lights.h"
#include "OBJMesh.h"
#include "Shader.h"
#include <map>

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
	void ImGUI_Functions(float _windowWidth, float _windowHeight);

	void AddPointLights(Lights _light) { m_pointLights.push_back(_light); };
	void AddShader(aie::ShaderProgram* _newShader, std::string _name);
	void AddMesh(aie::OBJMesh* _newMesh, std::string _name);

	BaseCamera* GetCamera() { return m_camera; }
	glm::vec2 GetWindowSize() { return m_windowSize; }
	glm::vec3 GetAmbeintLightColor() { return m_globalLight.color * m_globalLight.intensity * 0.25f; }
	void SetAmbientLightColor(glm::vec3 _color) { m_globalLight.color = _color; }

	Lights& GetGlobalLight() { return m_globalLight; }

	std::vector<Lights>& GetPointLights() { return m_pointLights; }
	int GetNumberOfLights() { return m_pointLights.size(); }
	int GetPostProcess() { return m_postProcess; }
	int GetPixelAmount() { return m_pixel; }
	int GetScaneSize() { return m_scaneSize; }
	int GetFogDist() { return m_distFog; }
	float GetScanlineDist() { return m_scanlineDist; }
	float GetEdgeDetection() { return m_edgeDetection; }
	float GetColorDif() { return m_colorDifference; }
	float GetWhiteColorDif() { return m_whiteColor; }
	glm::vec2 GetSinVal() { return m_sinVal; }

	glm::vec3* GetPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* GetPointLightColors() { return &m_pointLightColors[0]; }



protected:
	float m_edgeDetection = .5f;
	float m_colorDifference = 1;
	float m_whiteColor = 0.5f;
	float m_scanlineDist = 0;
	int m_scaneSize = 1000;
	int m_pixel = 1;
	int m_distFog = 1;
	int m_postProcess = 0;
	glm::vec2 m_sinVal = glm::vec2(0);
	
	bool openMenu;


	bool m_addLight;
	bool m_addObjects1;

	std::map<std::string, bool> m_objects;

	
	// Object 
	std::vector<aie::ShaderProgram*> m_currentShader;
	std::vector<std::string> m_shaderNames;
	std::vector<aie::OBJMesh*> m_currentMesh;

	BaseCamera* m_camera;
	glm::vec2 m_windowSize;
	std::list<Instance*> m_instances;
	std::vector<Lights> m_pointLights;
	Lights m_globalLight;


	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColors[MAX_LIGHTS];
};

