#pragma once

#include "BaseCamera.h"

class OrbitalCamera : public BaseCamera
{
public:
	void Update(float _deltaTime) override;
	void SetTarget(glm::vec3 pos, float distanceToTarget);

private:
	float m_distanceToTarget;
	float m_orbitalSpeed;
	glm::vec3 rotatePos;
};

