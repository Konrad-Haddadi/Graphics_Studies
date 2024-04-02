#include "BaseCamera.h"
#include <glm/ext.hpp>

BaseCamera::BaseCamera()
{
	m_aspectRatio = 16.f / 9.f;
	m_position = glm::vec3(-10, 2, 0);
	m_theta = 0;
	m_phi = 0;
	m_nearRender = 0.00001f;
	m_farRender = 1000.f;
}

void BaseCamera::CalculationUpdate(float _deltaTime)
{
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	m_forward = glm::vec3(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));
	m_viewTransform = glm::lookAt(m_position, m_position + m_forward, glm::vec3(0, 1, 0));

	Update(_deltaTime);
}

glm::mat4 BaseCamera::GetTransform(glm::vec3 _pos, glm::vec3 _angle, glm::vec3 _scale)
{
	return glm::translate(glm::mat4(1), _pos) *
		glm::rotate(glm::mat4(1), glm::radians(_angle.z), glm::vec3(0, 0, 1)) *
		glm::rotate(glm::mat4(1), glm::radians(_angle.y), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1), glm::radians(_angle.x), glm::vec3(1, 0, 0)) *
		glm::scale(glm::mat4(1), _scale);
}

glm::mat4 BaseCamera::GetProjectionMatrix(glm::vec2 _aspectRatio)
{
	return  glm::perspective(glm::pi<float>() * 0.25f, _aspectRatio.x / _aspectRatio.y, m_nearRender, m_farRender);
}

glm::mat4 BaseCamera::GetProjectionMatrix()
{
	return  glm::perspective(glm::pi<float>() * 0.25f, m_aspectRatio, m_nearRender, m_farRender);
}

void BaseCamera::SetViewMatrix(glm::vec3 from, glm::vec3 to, glm::vec3 up)
{
	m_viewTransform = glm::lookAt(from, to, up);
}

void BaseCamera::SetCenter(glm::vec3 center)
{
	m_viewTransform = glm::lookAt(m_position, center, glm::vec3(0,1,0));
}

void BaseCamera::SetProjectionMatrix(float fieldOfView, float aspectRatio, float _nearRender, float _farRender)
{	
	m_aspectRatio = aspectRatio;
	m_nearRender = _nearRender;
	m_farRender = _farRender;	
}

void BaseCamera::SetProjectionMatrix(float fieldOfView, float width, float height, float, float)
{
}
