#include "Scene.h"
#include "BaseCamera.h"
#include "Instance.h"
#include <imgui_glfw3.h>
#include <string>
#include "Gizmos.h"
Scene::Scene(BaseCamera* _camera, glm::vec2 _windowSize, Light& _globalLight)
	: m_camera(_camera), m_windowSize(_windowSize), m_globalLight(_globalLight)
{
	
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
}

void Scene::ImGUI_Functions()
{
	ImGui::Begin("Spear");	

	for (int i = 0; i < m_pointLights.size(); i++)
	{
		Light light = m_pointLights[i];

		std::string name = "Light ";
		name += i + 1 + 48;		
		ImGui::Text(name.c_str());

		name += ": ";

		std::string nameColor = name + "Color";
		std::string namePos = name + "Position";
		std::string nameIntensity = name + "Intensity";

		ImGui::SliderFloat3(nameColor.c_str(), &light.color.x, 0, 1);
		ImGui::SliderFloat3(namePos.c_str() , &light.direction.x, -10, 10);
		ImGui::SliderFloat(nameIntensity.c_str() , &light.intensity, 0, 50);
		m_pointLights[i] = light;

		aie::Gizmos::addSphere(light.direction , light.intensity / 50, 10, 10, glm::vec4(light.color.x, light.color.y, light.color.z, light.intensity));
	}
	
	

	ImGui::End();
}
