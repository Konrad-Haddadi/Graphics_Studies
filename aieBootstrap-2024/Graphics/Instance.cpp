#include "Instance.h"
#include "Shader.h"
#include "Texture.h"
#include "OBJMesh.h"
#include "Scene.h"
#include "BaseCamera.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <imgui_glfw3.h>
#include <string>

Instance::Instance(glm::mat4 _transform, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader, bool _isUntextured)
	: m_transform(_transform), m_mesh(_mesh), m_shader(_shader), m_isUntextured(_isUntextured), remove(false)
{
}

Instance::Instance(glm::vec3 _pos, glm::vec3 _eulerAngels, glm::vec3 _scale, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader, bool _isUntextured)
	: m_mesh(_mesh), m_shader(_shader), m_isUntextured(_isUntextured), remove(false)
{
	m_transform = Instance::MakeTransform(_pos, _eulerAngels, _scale);
}

Instance::~Instance()
{
}

void Instance::Draw(Scene* _scene)
{
	BaseCamera* bc = _scene->GetCamera();
	Lights lights = _scene->GetGlobalLight();
	
	auto pv = bc->GetProjectionMatrix(_scene->GetWindowSize()) * bc->GetViewMatrix();

	m_shader->bind();
	m_shader->bindUniform("ProjectionViewModel", pv * m_transform);
	m_shader->bindUniform("ModelMatrix", m_transform);
			
	m_shader->bindUniform("diffuseTexture", 0);
	m_shader->bindUniform("specularTexture", 0);
	m_shader->bindUniform("normalTexture", 0);
			
	m_shader->bindUniform("CameraPosition", bc->GetPosition());
			
	m_shader->bindUniform("LightDirection", _scene->GetGlobalLight().direction);
	m_shader->bindUniform("ambientLight", _scene->GetAmbeintLightColor());
	m_shader->bindUniform("diffuseLight", _scene->GetAmbeintLightColor());

	int numberOfLights = _scene->GetNumberOfLights();

	m_shader->bindUniform("NumberOfLights", numberOfLights);

	m_shader->bindUniform("PointLightPositions", numberOfLights, _scene->GetPointLightPositions());
	m_shader->bindUniform("PointLightColors", numberOfLights, _scene->GetPointLightColors());

	m_mesh->draw();
}

void Instance::ImGUI_Functions(std::string _name, bool _canRemove)
{
	std::string name = _name;

	ImGui::Text(name.c_str());

	name += ": ";

	std::string nameColor = name + "Color";
	std::string namePos = name + "Position";
	std::string nameIntensity = name + "Intensity";

	if (_canRemove)
	{
		std::string nameRemove = "Remove " + name;
		ImGui::Checkbox(nameRemove.c_str(), &remove);
	}

	glm::vec4* pos = &m_transform[3];

	ImGui::SliderFloat3(namePos.c_str(), &pos->x, -50, 50);
	ImGui::Text(" ");
}

glm::mat4 Instance::MakeTransform(glm::vec3 _pos, glm::vec3 _angle, glm::vec3 _scale)
{
	return glm::translate(glm::mat4(1), _pos) *
		glm::rotate(glm::mat4(1), glm::radians(_angle.z), glm::vec3(0, 0, 1)) *
		glm::rotate(glm::mat4(1), glm::radians(_angle.y), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1), glm::radians(_angle.x), glm::vec3(1, 0, 0)) *
		glm::scale(glm::mat4(1), _scale);
}
