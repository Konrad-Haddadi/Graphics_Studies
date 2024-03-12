#pragma once

#include "BaseCamera.h"

class FlyCamera : public BaseCamera
{
public:

	void Update(float _deltaTime) override;
	void SetSpeed(float _speed) { m_speed = _speed; }

private:
	float m_speed = glm::radians(180.f);
	vec3 m_up = vec3(0,1,0);
};

