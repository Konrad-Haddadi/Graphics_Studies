#include "StationaryCamera.h"
#include <Input.h>
#include "glm/ext.hpp"

void StationaryCamera::Update(float _deltaTime)
{
	aie::Input* input = aie::Input::getInstance();

	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	glm::vec3 right(-glm::sin(thetaR), 0, glm::cos(thetaR));

	// Implemented WASD movement;

	glm::vec3 move = glm::vec3(0);

	if (input->isKeyDown(aie::INPUT_KEY_W))
		move += glm::vec3(15,0,0);

	if (input->isKeyDown(aie::INPUT_KEY_S))
		move -= glm::vec3(15, 0, 0);

	if (input->isKeyDown(aie::INPUT_KEY_A))
		move -= glm::vec3(0, 15, 0);

	if (input->isKeyDown(aie::INPUT_KEY_D))
		move += glm::vec3(0, 15, 0);

	SetRotation(move);
}

void StationaryCamera::SetRotation(glm::vec3 _rotation)
{
	m_theta = _rotation.y;
	m_phi = _rotation.x;
}
