#include "SimpleCamera.h"
#include <glm/ext.hpp>
#include <Input.h>

SimpleCamera::SimpleCamera()
	: m_nearRender(0.1f), m_farRender(1000.f)
{
	m_position = glm::vec3(-10, 2, 0);
	m_phi = 0;
	m_theta = 0;
}


void SimpleCamera::Update(float _dt)
{
	aie::Input* input = aie::Input::getInstance();

	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR)); 
	
	glm::vec3 right(-glm::sin(thetaR), 0, glm::cos(thetaR));
	glm::vec3 up(0, 1, 0);

	// Implemented WASD movement;

	glm::vec3 move = glm::vec3(0);

	if (input->isKeyDown(aie::INPUT_KEY_W))
		move += forward * _dt;

	if (input->isKeyDown(aie::INPUT_KEY_S))
		move -= forward * _dt;

	if (input->isKeyDown(aie::INPUT_KEY_A))
		move -= right * _dt;

	if (input->isKeyDown(aie::INPUT_KEY_D))
		move += right * _dt;

	glm::vec3 moveUp = glm::vec3(0);

	if (input->isKeyDown(aie::INPUT_KEY_Q))
		moveUp += up * _dt;

	if (input->isKeyDown(aie::INPUT_KEY_E))
		moveUp -= up * _dt;

	float speed = 1;

	if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
		speed = 2;

	m_position += move * speed;
	m_position += moveUp * speed;

	float mx = input->getMouseX();
	float my = input->getMouseY();

	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_theta += m_turnSpeed * (mx - m_lastMousePos.x) * _dt;
		m_phi += m_turnSpeed * (my - m_lastMousePos.y) * _dt;
	}

	m_lastMousePos = glm::vec2(mx,my);
}

glm::mat4 SimpleCamera::GetViewMatrix()
{
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));


	return glm::lookAt(m_position, m_position + forward, glm::vec3(0,1,0));
}

glm::mat4 SimpleCamera::GetProjectionMatrix(float _width, float _height)
{
	return glm::perspective(glm::pi<float>() * 0.25f, _width / _height, m_nearRender, m_farRender);
}

glm::mat4 SimpleCamera::GetTransform(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale)
{
	return glm::mat4();
}
