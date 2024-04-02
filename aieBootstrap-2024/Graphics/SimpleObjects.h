#pragma once
#include "GameObject.h"

class Mesh;

class SimpleObjects : public GameObject
{
public:
	SimpleObjects(glm::mat4 _transform, Mesh* _mesh, aie::ShaderProgram* _shader, std::string _name);
	SimpleObjects(glm::vec3 _pos, glm::vec3 _eulerAngels, glm::vec3 _scale, Mesh* _mesh, aie::ShaderProgram* _shader, std::string _name);
	~SimpleObjects();

	void Draw(Scene* _scene) override;
	void ImGUI_Functions(std::string _addToName, bool _canRemove) override;

protected:
	glm::vec4 m_color;
};

