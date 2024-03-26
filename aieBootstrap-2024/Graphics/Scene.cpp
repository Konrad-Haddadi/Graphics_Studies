#include "Scene.h"
#include "BaseCamera.h"
#include "GameObject.h"
#include "Instance.h"
#include "SimpleObjects.h"
#include <imgui_glfw3.h>
#include <string>
#include <vector>
#include <map>
#include "Gizmos.h"
#include "Lights.h"

Scene::Scene(BaseCamera* _camera, glm::vec2 _windowSize, Lights& _globalLight)
	: m_camera(_camera), m_windowSize(_windowSize), m_globalLight(_globalLight)
{
	m_addLight = false;

	m_addObjects = false;
	
	openMenu = false;
}

Scene::~Scene()
{
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		delete* it;
	}
}

void Scene::AddGameObject(GameObject* _instance)
{
	m_gameObjects.push_back(_instance);	
}

void Scene::Update(float _dt)
{
	m_camera->CalculationUpdate(_dt);

	if (m_addLight)
	{
		if(m_pointLights.size() < 4)
			GetPointLights().push_back(Lights(glm::vec3(0, 2, 0), glm::vec3(1, 1, 1), 10.f));

		m_addLight = false;
	}

	std::map<std::string, bool>::iterator it = m_objects.begin();

	int meshCount = m_objects.size() - 1;

	while (it != m_objects.end())
	{
		if (m_objects.at(it->first))
		{			
			if (it->first[0] == 'C')
			{
				AddGameObject(new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1), m_complexMesh[0], m_shaderType[0], it->first));
				m_objects.at(it->first) = false;
			}
			else
			{
				AddGameObject(new SimpleObjects(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1), m_simpleMesh[0], m_shaderType[1], it->first));
				m_objects.at(it->first) = false;
			}
			
		}

		it++;
		meshCount--;
	}

	// Last function in our Update;
	

	for (int i = 0; i < MAX_LIGHTS && i < GetNumberOfLights(); i++)
	{
		if (m_pointLights[i].remove)
			m_pointLights.erase(m_pointLights.begin() + i);
	}

	if (m_gameObjects.size() > 0)
	{
		for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
		{
			GameObject* instance = *it;

			if (instance->remove)
			{
				m_gameObjects.remove(*it);
				return;
			}
		}
	}

}

void Scene::Draw()
{
	for (int i = 0; i < MAX_LIGHTS && i < GetNumberOfLights(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].direction;
		m_pointLightColors[i] = m_pointLights[i].GetColor();
	}

	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		GameObject* instance = *it;
		instance->Draw(this);
	}	
}

void Scene::ImGUI_Functions(float _windowWidth, float _windowHeight)
{
	ImGui::Begin("Insepctor");	
	ImGui::SetWindowPos({ 0,0 });
	ImGui::SetWindowSize({ _windowWidth / 5,	_windowHeight - _windowHeight / 5});

	ImGui::InputInt("Set Post Process Val:", &m_postProcess);

	switch (m_postProcess)
	{
	case 3:
		ImGui::DragFloat("Set Edge Detection:", &m_edgeDetection, .01f, 0, 3);
		break;

	case 5:
		ImGui::DragInt("Set Scan Size:", &m_scaneSize, 10, 0, 1000);

		break;
	case 8:
		ImGui::InputInt("Set Pixel Amount:", &m_pixel, 1);
		break;

	case 9:
		ImGui::DragFloat("Set Color Dif:", &m_colorDifference, 0.01f, 1, 5);
		break;

	case 10:
		ImGui::InputInt("Set Distance:", &m_distFog, 1);

		break;
	case 12:
		ImGui::DragFloat("Set White Color Dif:", &m_whiteColor, 0.01f, 0, 1);
		ImGui::DragFloat("Set Edge Detection:", &m_edgeDetection, .01f, 0, 3);

		break;

	case 13:
		ImGui::DragInt("Set Scan Size:", &m_scaneSize, 10, 0, 1000);
		ImGui::InputInt("Set Pixel Amount:", &m_pixel, 1);
		break;

	case 14:
		ImGui::InputInt("Set Distance:", &m_distFog, 1);

		break;
	}
	
	ImGui::Text("");
	m_globalLight.ImGUI_Functions("Global Light", false);

	ImGui::Text("Objects");

	std::map<std::string, bool>::iterator it = m_objects.begin();

	while (it != m_objects.end())
	{
		std::string name = it->first.c_str();
		ImGui::Checkbox(name.c_str(), &m_objects.at(name));
		it++;
	}

	ImGui::End();

	ImGui::Begin("Lights");
	ImGui::SetWindowPos({ 0,_windowHeight - _windowHeight / 5 });
	ImGui::SetWindowSize({ _windowWidth - _windowWidth / 5,	_windowHeight /5});

	
	ImGui::Text("");
	ImGui::Checkbox("Add Light", &m_addLight);
	ImGui::Text("");

	for (int i = 0; i < m_pointLights.size(); i++)
	{
		std::string name = "Light ";
		name += i + 1 + 48;

		m_pointLights[i].ImGUI_Functions(name);
	}
	ImGui::End();

	
	ImGui::Begin("Objects");
	ImGui::SetWindowPos({ _windowWidth - _windowWidth / 5,0 });
	ImGui::SetWindowSize({ _windowWidth / 5,	_windowHeight });
	int i = 0;
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		GameObject* instance = *it;

		std::string name = " ";
		name += i + 1 + 48;

		instance->ImGUI_Functions(name);
		i++;
	}

	ImGui::End();
	
}

void Scene::AddShader(aie::ShaderProgram* _newShader, std::string _name)
{
	m_shaderType.push_back(_newShader);
	m_shaderNames.push_back(_name);
}

void Scene::AddSimpleShader(aie::ShaderProgram* _newShader, std::string _name)
{
	m_shaderType.push_back(_newShader);
	m_shaderNames.push_back(_name);
}

void Scene::AddComplexMesh(aie::OBJMesh* _newMesh, std::string _name)
{
	m_complexMesh.push_back(_newMesh); 
	m_objects.insert({_name, false});
}

void Scene::AddSimpleMesh(Mesh* _newMesh, std::string _name)
{
	m_simpleMesh.push_back(_newMesh);
	m_objects.insert({ _name, false });
}