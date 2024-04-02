#pragma once

#pragma once
#include <glm/glm.hpp>
#include <string>

class BaseCamera;
class Scene;
class Mesh;
struct Light;

namespace aie {
	class OBJMesh;
	class ShaderProgram;
}

class GameObject
{
public:
	GameObject(glm::mat4 _transform, aie::ShaderProgram* _shader, std::string _name, Mesh* _simpleMesh = nullptr);
	GameObject(glm::vec3 _pos, glm::vec3 _eulerAngels, glm::vec3 _scale, aie::ShaderProgram* _shader, std::string _name, Mesh* _simpleMesh = nullptr);
	~GameObject();

	virtual void Draw(Scene* _scene) = 0;
	virtual void ImGUI_Functions(std::string _addToName = "", bool _canRemove = true) = 0;

	static glm::mat4 MakeTransform(glm::vec3 _pos, glm::vec3 _angle, glm::vec3 _scale);

public:
	bool remove;
	std::string name;

protected:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	
	aie::ShaderProgram* m_shader;
	glm::mat4 m_transform;
	Mesh* m_simpleMesh;


};