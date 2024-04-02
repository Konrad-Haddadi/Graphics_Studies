#pragma once

#include <glm/glm.hpp>


class BaseCamera
{
public:
	BaseCamera();
	~BaseCamera() {};

	virtual void Update(float _deltaTime) = 0;

	void CalculationUpdate(float _deltaTime);

	glm::vec3 GetPosition() { return m_position; }

	glm::mat4 GetTransform(glm::vec3 _pos, glm::vec3 _angle, glm::vec3 _scale);
	glm::mat4 GetProjectionViewMatrix() { return m_projectViewTransform; }
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetProjectionMatrix(glm::vec2 _aspectRatio);
	glm::mat4 GetViewMatrix() { return m_viewTransform; }

	float GetAspectRatio() { return m_aspectRatio; }

	void SetPosition(glm::vec3 position) { m_position = position; }
	void SetAspectRatio(float width, float height) { m_aspectRatio = width / height; }
	void SetViewMatrix(glm::vec3 from, glm::vec3 to, glm::vec3 up);
	void SetProjectionMatrix(float fieldOfView, float aspectRatio, float _nearRender, float _farRender);
	void SetProjectionMatrix(float fieldOfView, float width, float height, float _nearRender, float _farRender);
	void SetCenter(glm::vec3 center);
protected:
	glm::mat4 m_projectViewTransform;
	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;

	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec2 m_lastMouse;

	float m_aspectRatio;
	float m_theta;
	float m_phi;
	float m_nearRender;
	float m_farRender;


};

