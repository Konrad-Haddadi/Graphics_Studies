#include "SimpleObjects.h"
#include "Shader.h"
#include "Mesh.h"
#include "Scene.h"
#include "BaseCamera.h"
#include <imgui_glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

SimpleObjects::SimpleObjects(glm::mat4 _transform, Mesh* _mesh, aie::ShaderProgram* _shader, std::string _name)
	: GameObject(_transform, _shader, _name, _mesh)
{
}


SimpleObjects::SimpleObjects(glm::vec3 _pos, glm::vec3 _eulerAngels, glm::vec3 _scale, Mesh* _mesh, aie::ShaderProgram* _shader, std::string _name)
	: GameObject(_pos, _eulerAngels, _scale, _shader, _name, _mesh)
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
	m_shader->bindUniform("BaseColour", glm::vec4(1, 1, 1, 0.5f));
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

	float scaleSetter = 0;

	glm::vec3 pos = glm::vec3(0);
	glm::vec3 rot = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);

	glm::vec3 newPos = m_transform[3];
	glm::vec3 oldPos = m_transform[3];

	ImGui::InputFloat3(namePos.c_str(), &newPos.x, 0);

	if (newPos != oldPos)
		m_transform = glm::translate(glm::mat4(1), newPos);


	ImGui::SliderFloat3(nameMove.c_str(), &pos.x, -1, 1);
	ImGui::SliderFloat3(nameRot.c_str(), &rot.x, -1, 1);
	ImGui::SliderFloat3(nameScale.c_str(), &scale.x, 0.9f, 1.1f);

	ImGui::InputFloat(nameScaleOverAll.c_str(), &scaleSetter);
	ImGui::Text(" ");

	pos = pos * 0.1f;

	m_transform *= glm::translate(glm::mat4(1), pos);

	m_transform *= glm::rotate(glm::mat4(1), glm::radians(rot.z), glm::vec3(0, 0, 1)) *
		glm::rotate(glm::mat4(1), glm::radians(rot.y), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1), glm::radians(rot.x), glm::vec3(1, 0, 0));

	m_transform *= glm::scale(glm::mat4(1), scale);

	if (scaleSetter != 0)
		m_transform = glm::scale(glm::mat4(1), glm::vec3(scaleSetter));
}
