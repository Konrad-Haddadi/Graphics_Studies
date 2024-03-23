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

class Instance
{
public:
	Instance(glm::mat4 _transform, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader, std::string _name, bool _invertLightDir = false, bool _isUntextured = false);
	Instance(glm::vec3 _pos, glm::vec3 _eulerAngels, glm::vec3 _scale, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader, std::string _name, bool _invertLightDir = false, bool _isUntextured = false);
	~Instance();

	void Draw(Scene* _scene);
	void ImGUI_Functions(std::string _addToName = "", bool _canRemove = true);

	static glm::mat4 MakeTransform(glm::vec3 _pos, glm::vec3 _angle, glm::vec3 _scale);

public:
	bool remove;
	int layerSample;
	std::string name;

protected:
	glm::mat4 m_transform;
	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shader;
	Mesh* m_simpleMesh;

	bool m_invertLight;
	bool m_isUntextured;
};

