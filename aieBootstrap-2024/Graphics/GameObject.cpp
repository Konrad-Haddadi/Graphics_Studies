#include "GameObject.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <imgui_glfw3.h>

GameObject::GameObject(glm::mat4 _transform, aie::ShaderProgram* _shader, std::string _name, Mesh* _simpleMesh)
: m_transform(_transform), m_shader(_shader), name(_name), remove(false), m_simpleMesh(_simpleMesh), m_position(glm::vec3(0)), m_rotation(glm::vec3(0)), m_scale(glm::vec3(1))
{
}

GameObject::GameObject(glm::vec3 _pos, glm::vec3 _eulerAngels, glm::vec3 _scale, aie::ShaderProgram* _shader, std::string _name, Mesh* _simpleMesh)
: m_shader(_shader), name(_name), remove(false), m_simpleMesh(_simpleMesh), m_position(_pos), m_rotation(_eulerAngels), m_scale(_scale)
{
	m_transform = MakeTransform(m_position, m_rotation, m_scale);
}

GameObject::~GameObject()
{
}

glm::mat4 GameObject::MakeTransform(glm::vec3 _pos, glm::vec3 _angle, glm::vec3 _scale)
{
	return glm::translate(glm::mat4(1), _pos) * 
		glm::rotate(glm::mat4(1), glm::radians(_angle.z), glm::vec3(0, 0, 1)) *
		glm::rotate(glm::mat4(1), glm::radians(_angle.y), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1), glm::radians(_angle.x), glm::vec3(1, 0, 0)) *
		glm::scale(glm::mat4(1), _scale);
}
