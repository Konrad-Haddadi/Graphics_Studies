#include "Scene.h"
#include "BaseCamera.h"
#include "Instance.h"
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
	m_postProcess = 0;

	m_addObjects1 = false;
	
	openMenu = false;
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
			AddInstance(new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1), m_currentMesh[meshCount], m_currentShader[0], it->first));
			m_objects.at(it->first) = false;
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

	if (m_instances.size() > 0)
	{
		for (auto it = m_instances.begin(); it != m_instances.end(); it++)
		{
			Instance* instance = *it;

			if (instance->remove)
			{
				m_instances.remove(*it);
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

	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		Instance* instance = *it;
		instance->Draw(this);
	}	
}

void Scene::ImGUI_Functions(float _windowWidth, float _windowHeight)
{
	ImGui::Begin("Insepctor");	
	ImGui::SetWindowPos({ 0,0 });
	ImGui::SetWindowSize({ _windowWidth / 4,	_windowHeight});

	ImGui::InputInt("Set Post Process Val:", &m_postProcess);
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


	ImGui::BeginChild(ImGuiID(1), { 0,0 }, true);
	ImGui::Text("Lights");
	ImGui::Text("");
	ImGui::Checkbox("Add Light", &m_addLight);
	ImGui::Text("");

	for (int i = 0; i < m_pointLights.size(); i++)
	{
		std::string name = "Light ";
		name += i + 1 + 48;

		m_pointLights[i].ImGUI_Functions(name);
	}

	ImGui::EndChild();

	ImGui::End();	

	if (m_instances.size() > 0)
	{
		ImGui::Begin("Objects");
		ImGui::SetWindowPos({ _windowWidth - _windowWidth / 4,0 });
		ImGui::SetWindowSize({ _windowWidth / 4,	_windowHeight });
		int i = 0;
		for (auto it = m_instances.begin(); it != m_instances.end(); it++)
		{
			Instance* instance = *it;

			std::string name = " ";
			name += i + 1 + 48;

			instance->ImGUI_Functions(name);
			i++;
		}

		ImGui::End();
	}
	



	/*ImGui::BeginChild(ImGuiID(1), { 0,250 }, true);
	ImGui::Text("Lights");


	ImGui::Text("");

	ImGui::Checkbox("Add Light", &m_addLight);

	for (int i = 0; i < m_pointLights.size(); i++)
	{
		std::string name = "Light ";
		name += i + 1 + 48;

		m_pointLights[i].ImGUI_Functions(name);
	}

	ImGui::EndChild();	



	ImGui::BeginChild(ImGuiID(2), {0,250}, true);

	ImGui::Text("Objects");

	std::map<std::string, bool>::iterator it = m_objects.begin();

	while (it != m_objects.end())
	{
		std::string name = it->first.c_str();
		ImGui::Checkbox(name.c_str(), &m_objects.at(name));
		it++;
	}

	ImGui::Text(" ");

	int i = 0;
	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		Instance* instance = *it;
		
		std::string name = " ";
		name += i + 1 + 48;

		instance->ImGUI_Functions(name); 
		i++;
	}

	ImGui::EndChild();
	ImGui::End();*/

}

void Scene::AddShader(aie::ShaderProgram* _newShader, std::string _name)
{
	m_currentShader.push_back(_newShader);
	m_shaderNames.push_back(_name);
}

void Scene::AddMesh(aie::OBJMesh* _newMesh, std::string _name)
{
	m_currentMesh.push_back(_newMesh); 
	m_objects.insert({_name, false});
}
