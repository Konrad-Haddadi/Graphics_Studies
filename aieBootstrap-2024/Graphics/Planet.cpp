#include "Planet.h"

#include <Gizmos.h>

Planet::Planet(glm::vec3 _position, float _size, glm::vec4 _color, Planet* _orbiting, bool _hasMoon, bool _hasRing)
	: position(_position), size(_size), color(_color), orbit(_orbiting), hasMoon(_hasMoon), hasRing(_hasRing), moon(nullptr)
{
	transform = glm::mat4(_position.x, 0 , 0, 0,
							0, _position.y, 0,0,
							0,0,_position.z,0,
							0,0,-1,0);

	
	if (hasMoon)
	{
		moon = new Planet(position + glm::vec3(1, 0, 0), size / 2, glm::vec4(.5f, .5f, .5f, 1), this);
	}
}

Planet::~Planet()
{
}

void Planet::Update(float _dt)
{
	if (orbit)
	{
		glm::vec3 rotation = position - orbit->position;
		position += glm::vec3(rotation.z, position.y , -rotation.x) * _dt;
	}

	if (moon)
		moon->Update(_dt);
}

void Planet::Draw()
{
	aie::Gizmos::addSphere(position, size, 10, 10, color);

	if (moon)
		moon->Draw();
}


