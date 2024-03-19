#pragma once
#include <glm/glm.hpp>

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
	Instance(glm::mat4 _transform, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader, bool _isUntextured = false);
	Instance(glm::vec3 _pos, glm::vec3 _eulerAngels, glm::vec3 _scale, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader, bool _isUntextured = false);
	~Instance();

	void Draw(Scene* _scene);

	static glm::mat4 MakeTransform(glm::vec3 _pos, glm::vec3 _angle, glm::vec3 _scale);

protected:
	glm::mat4 m_transform;
	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shader;
	Mesh* m_simpleMesh;

	bool m_isUntextured;
};

