#pragma once
#include <glm/glm.hpp>
#include <string>
#include "GameObject.h"

class BaseCamera;
class Scene;
class Mesh;
struct Light;

namespace aie {
	class OBJMesh;
	class ShaderProgram;
}

class Instance : public GameObject
{
public:
	Instance(glm::mat4 _transform, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader, std::string _name, bool _invertLightDir = false, bool _isUntextured = false);
	Instance(glm::vec3 _pos, glm::vec3 _eulerAngels, glm::vec3 _scale, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader, std::string _name, bool _invertLightDir = false, bool _isUntextured = false);
	~Instance();

	virtual void Draw(Scene* _scene) override;
	virtual void ImGUI_Functions(std::string _addToName = "", bool _canRemove = true) override;
	 
public:
	int layerSample;

protected:
	aie::OBJMesh* m_mesh;
	bool m_invertLight;
	bool m_isUntextured;
};

