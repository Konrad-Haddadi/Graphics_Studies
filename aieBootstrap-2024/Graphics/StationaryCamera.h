#pragma once

#include "BaseCamera.h"
#include <glm/glm.hpp>

class StationaryCamera : public BaseCamera
{
public:
	void Update(float _deltaTime) override;
	void SetRotation(glm::vec3 _rotation);
};

