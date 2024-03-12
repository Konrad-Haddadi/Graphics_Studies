#include "FlyCamera.h"
#include "Input.h"

void FlyCamera::Update(float _deltaTime)
{
	aie::Input* input = aie::Input::getInstance();

	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	glm::vec3 right(-glm::sin(thetaR), 0, glm::cos(thetaR));

	// Implemented WASD movement;

	glm::vec3 move = glm::vec3(0);

	if (input->isKeyDown(aie::INPUT_KEY_W))
		move += m_forward * _deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_S))
		move -= m_forward * _deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_A))
		move -= right * _deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_D))
		move += right * _deltaTime;

	glm::vec3 moveUp = glm::vec3(0);

	if (input->isKeyDown(aie::INPUT_KEY_Q))
		moveUp += m_up * _deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_E))
		moveUp -= m_up * _deltaTime;

	float speed = 1;

	if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
		speed = 2;

	m_position += move * speed;
	m_position += moveUp * speed;

	float mx = input->getMouseX();
	float my = input->getMouseY();

	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_theta += m_speed * (mx - m_lastMouse.x) * _deltaTime;
		m_phi += m_speed * (my - m_lastMouse.y) * _deltaTime;
	}

	m_lastMouse = glm::vec2(mx, my);
}
