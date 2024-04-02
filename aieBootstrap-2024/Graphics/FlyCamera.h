#pragma once

#include "BaseCamera.h"

class FlyCamera : public BaseCamera
{
public:

	void Update(float _deltaTime) override;
	void SetSpeed(float _speed) { m_speed = _speed; }

private:
	float m_speed = glm::radians(360.f);
	glm::vec3 m_up = glm::vec3(0,1,0);
};

