#pragma once

#include <glm/glm.hpp>

class Planet
{
public:
	Planet(glm::vec3 _position, float _size,glm::vec4 _color, Planet* _orbiting = nullptr, bool _hasMoon = false, bool _hasRing = false);
	~Planet();

	void Update(float _dt);
	void Draw();


public:
	glm::mat4 transform;
	glm::vec3 position;
	float size;

	glm::vec4 color;

	Planet* orbit;
	Planet* moon;

	bool hasMoon;
	bool hasRing;
};

