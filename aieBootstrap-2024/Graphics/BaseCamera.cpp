#include "BaseCamera.h"
#include <glm/ext.hpp>

BaseCamera::BaseCamera()
{
	m_aspectRatio = 16.f / 9.f;
	m_position = vec3(-10, 2, 0);
	m_theta = 0;
	m_phi = 0;
	m_nearRender = 0.00001f;
	m_farRender = 1000.f;
}

void BaseCamera::CalculationUpdate(float _deltaTime)
{
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	m_forward = vec3(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));

	m_viewTransform = glm::lookAt(m_position, m_position + m_forward, glm::vec3(0, 1, 0));

	Update(_deltaTime);
}

mat4 BaseCamera::GetWorldTransform(vec3 cameraPosition, vec3 eularAngels, vec3 scale)
{
	return mat4();
}

mat4 BaseCamera::GetProjectionMatrix()
{
	return  glm::perspective(glm::pi<float>() * 0.25f, m_aspectRatio, m_nearRender, m_farRender);
}

void BaseCamera::SetViewMatrix(vec3 from, vec3 to, vec3 up)
{
}

void BaseCamera::SetProjectionMatrix(float fieldOfView, float aspectRatio, float, float)
{
}

void BaseCamera::SetProjectionMatrix(float fieldOfView, float width, float height, float, float)
{
}
