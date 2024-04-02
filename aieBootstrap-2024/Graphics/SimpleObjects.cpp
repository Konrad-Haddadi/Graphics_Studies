#include "SimpleObjects.h"
#include "Shader.h"
#include "Mesh.h"
#include "Scene.h"
#include "BaseCamera.h"
#include <imgui_glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

SimpleObjects::SimpleObjects(glm::mat4 _transform, Mesh* _mesh, aie::ShaderProgram* _shader, std::string _name)
	: GameObject(_transform, _shader, _name, _mesh), m_color(glm::vec4(1))
{
}


SimpleObjects::SimpleObjects(glm::vec3 _pos, glm::vec3 _eulerAngels, glm::vec3 _scale, Mesh* _mesh, aie::ShaderProgram* _shader, std::string _name)
	: GameObject(_pos, _eulerAngels, _scale, _shader, _name, _mesh), m_color(glm::vec4(1))
{
}

SimpleObjects::~SimpleObjects()
{
}

void SimpleObjects::Draw(Scene* _scene)
{
	BaseCamera* bc = _scene->GetCamera();

	auto pv = bc->GetProjectionMatrix(_scene->GetWindowSize()) * bc->GetViewMatrix();

	m_shader->bind();
	m_shader->bindUniform("ProjectionViewModel", pv * m_transform);
	m_shader->bindUniform("BaseColour", m_color);
	m_simpleMesh->Draw();


}

void SimpleObjects::ImGUI_Functions(std::string _addToName, bool _canRemove)
{
	ImGui::Text(name.c_str());

	std::string nameAdd = name + _addToName + ": ";

	std::string nameLayer = nameAdd + "Layer";
	std::string nameColor = nameAdd + "Color";
	std::string nameMove = nameAdd + "Move Pos";
	std::string namePos = nameAdd + "Position";
	std::string nameRot = nameAdd + "Rotation";
	std::string nameScale = nameAdd + "Scale";
	std::string nameScaleOverAll = nameAdd + "Scale Setter";

	if (_canRemove)
	{
		std::string nameRemove = "Remove " + nameAdd; 
		ImGui::Checkbox(nameRemove.c_str(), &remove); 
	}

	ImGui::Text(""); 

	ImGui::InputFloat3(nameMove.c_str(), &m_position.x, -1, 1); 
	ImGui::InputFloat3(nameRot.c_str(), &m_rotation.x, -1, 1);
	ImGui::InputFloat3(nameScale.c_str(), &m_scale.x, 0.9f, 1.1f); 

	ImGui::DragFloat4(nameColor.c_str(), &m_color.x,0.01f, 0, 1);
	ImGui::Text(" ");

	m_transform = glm::mat4(1);

	m_transform *= glm::translate(glm::mat4(1), m_position);

	m_transform *= glm::rotate(glm::mat4(1), glm::radians(m_rotation.z), glm::vec3(0, 0, 1)) *
		glm::rotate(glm::mat4(1), glm::radians(m_rotation.y), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1), glm::radians(m_rotation.x), glm::vec3(1, 0, 0));

	m_transform *= glm::scale(glm::mat4(1), m_scale);
}
