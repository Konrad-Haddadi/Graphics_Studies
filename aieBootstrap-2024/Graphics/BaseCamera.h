#pragma once

#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;
using glm::mat4;

class BaseCamera
{
public:
	BaseCamera();
	~BaseCamera() {};

	virtual void Update(float _deltaTime) = 0;

	void CalculationUpdate(float _deltaTime);

	vec3 GetPosition() { return m_position; }

	mat4 GetWorldTransform(vec3 cameraPosition, vec3 eularAngels, vec3 scale);
	mat4 GetProjectionViewMatrix() { return m_projectViewTransform; }
	mat4 GetProjectionMatrix();
	mat4 GetProjectionMatrix(glm::vec2 _aspectRatio);
	mat4 GetViewMatrix() { return m_viewTransform; }

	float GetAspectRatio() { return m_aspectRatio; }

	void SetPosition(vec3 position) { m_position = position; }
	void SetAspectRatio(float width, float height) { m_aspectRatio = width / height; }
	void SetViewMatrix(vec3 from, vec3 to, vec3 up);
	void SetProjectionMatrix(float fieldOfView, float aspectRatio, float _nearRender, float _farRender);
	void SetProjectionMatrix(float fieldOfView, float width, float height, float _nearRender, float _farRender);
	void SetCenter(vec3 center);
protected:
	mat4 m_projectViewTransform;
	mat4 m_worldTransform;
	mat4 m_viewTransform;

	vec3 m_position;
	vec3 m_forward;
	vec2 m_lastMouse;

	float m_aspectRatio;
	float m_theta;
	float m_phi;
	float m_nearRender;
	float m_farRender;


};

