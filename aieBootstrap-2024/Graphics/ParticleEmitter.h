#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <gl_core_4_4.h>
#include <glm/glm.hpp>

using glm::vec3;
using glm::vec4;

class ParticleEmitter
{
public:
	struct Particle {
		vec3 position;
		vec3 velocity;
		vec4 color;

		float size;
		float lifetime;
		float lifespan;
	};

	struct ParticleVertex {
		vec4 position;
		vec4 color;
	};

public:
	ParticleEmitter();
	virtual ~ParticleEmitter();

	void Initialise(unsigned int _maxParticles, unsigned int _emitRate, float _lifetimeMin, float _lifetimeMax, 
		float _velocityMin, float _velocityMax, float _startSize, float _endSize, const vec4& _startColour, const vec4& _endColour);

	void Emit();
	void Update(float _dt, const glm::mat4& _cameraTransform);
	void Draw();
protected:

	Particle* m_particles;
	unsigned int m_firstDead;
	unsigned int m_maxParticles;

	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;
	ParticleVertex* m_vertexData;

	vec3 m_position;

	float m_emitTimer;
	float m_emitRate;

	float m_lifespanMin;
	float m_lifespanMax;

	float m_velocityMin;
	float m_velocityMax;

	float m_startSize;
	float m_endSize;

	vec4 m_startColor;
	vec4 m_endColor;
};

