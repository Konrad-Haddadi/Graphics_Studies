#pragma once

#include <glm/glm.hpp>

class Transform
{
public:
	Transform(glm::vec3 _position, glm::vec3 _rotation = glm::vec3(0), glm::vec3 _scale = glm::vec3(1));
	~Transform() {};

public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
};

