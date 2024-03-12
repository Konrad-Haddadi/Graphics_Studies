#include "OrbitalCamera.h"
#include <Input.h>

void OrbitalCamera::Update(float _deltaTime)
{
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_SPACE))
	{
		glm::vec3 rotation = m_position - rotatePos;
		m_position += glm::vec3(rotation.z, 0, -rotation.x) * _deltaTime;
	}

	

	m_theta = rotatePos.z - m_position.z * 1;
	m_phi = 0;
	
}

void OrbitalCamera::SetTarget(vec3 pos, float distanceToTarget)
{
	rotatePos = pos + vec3(distanceToTarget,0,0);
}


