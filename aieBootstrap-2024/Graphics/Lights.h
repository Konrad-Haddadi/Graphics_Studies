#pragma once

#include <glm/glm.hpp>
#include <string>

class Lights
{
public:
	Lights()
	{
		direction = glm::vec3(0, 0, 0);
		color = glm::vec3(1);
		intensity = 1;
		remove = false;
	}

	Lights(glm::vec3 _position, glm::vec3 _color = glm::vec3(1), float _intensity = 1)
		: direction(_position), color(_color), intensity(_intensity), remove(false)
	{	}

	void ImGUI_Functions(std::string _name, bool _canRemove = true);

public:
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
	bool remove;

	glm::vec3 GetColor() {	return color * intensity;	}
	
};

