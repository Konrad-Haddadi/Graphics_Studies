#include "Scene.h"
#include "BaseCamera.h"
#include "Instance.h"
#include <imgui_glfw3.h>
#include <string>
#include "Gizmos.h"
#include "Lights.h"

Scene::Scene(BaseCamera* _camera, glm::vec2 _windowSize, Lights& _globalLight)
	: m_camera(_camera), m_windowSize(_windowSize), m_globalLight(_globalLight)
{
	m_addLight = false;
}

Scene::~Scene()
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		delete* it;
	}
}

void Scene::AddInstance(Instance* _instance)
{
	m_instances.push_back(_instance);
}

void Scene::Update(float _dt)
{
	m_camera->CalculationUpdate(_dt);

	if (m_addLight)
	{
		GetPointLights().push_back(Lights(glm::vec3(0, 2, 0), glm::vec3(1, 1, 1), 10.f));
		m_addLight = false;
	}

	// Last function in our Update;
	ImGUI_Functions();
}

void Scene::Draw()
{
	for (int i = 0; i < MAX_LIGHTS && i < GetNumberOfLights(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].direction;
		m_pointLightColors[i] = m_pointLights[i].GetColor();
	}

	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		Instance* instance = *it;
		instance->Draw(this);
	}	

	for (int i = 0; i < MAX_LIGHTS && i < GetNumberOfLights(); i++)
	{
		if (m_pointLights[i].remove)
			m_pointLights.erase(m_pointLights.begin() + i);
	}
}

void Scene::ImGUI_Functions()
{
	ImGui::Begin("Spear");	

	ImGui::Checkbox("Add Light", &m_addLight);
	
	ImGui::Text("");

	m_globalLight.ImGUI_Functions("Global", false);

	for (int i = 0; i < m_pointLights.size(); i++)
	{
		std::string name = "Light ";
		name += i + 1 + 48;

		m_pointLights[i].ImGUI_Functions(name);
	}

	


	ImGui::End();
}
