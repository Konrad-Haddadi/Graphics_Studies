#pragma once

#include <glm/glm.hpp>

class SimpleCamera
{
public:
	SimpleCamera();
	~SimpleCamera() {};

	void Update(float _dt);

	glm::vec3 GetPosition() { return m_position; }

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float _width, float _height);

	glm::mat4 GetTransform(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale);
private:
	float m_theta; // In degrees
	float m_phi; // In degrees
	
	float m_turnSpeed = glm::radians<float>(180);

	float m_nearRender;
	float m_farRender;
	glm::vec3 m_position;
	glm::vec2 m_lastMousePos;

};

