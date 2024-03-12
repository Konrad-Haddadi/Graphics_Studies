#pragma once

#include "BaseCamera.h"

class OrbitalCamera : public BaseCamera
{
public:
	void Update(float _deltaTime) override;
	void SetTarget(vec3 pos, float distanceToTarget);

private:
	float m_distanceToTarget;
	float m_orbitalSpeed;
	vec3 rotatePos;
};

