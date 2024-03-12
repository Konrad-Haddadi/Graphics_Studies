#pragma once

#include "Application.h"
#include "FlyCamera.h"
#include "StationaryCamera.h"
#include "OrbitalCamera.h"
#include "Shader.h"
#include "Mesh.h"
#include "RenderTarget.h"

#include <glm/mat4x4.hpp>
#include <vector>

class GraphicsApp : public aie::Application {
public:

	GraphicsApp();
	virtual ~GraphicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();
	void ImGUI_Helper();
protected:

	bool LaunchShaders();
	bool LoadShaders(aie::ShaderProgram& _shaderToLoad, const char* _filePath, std::string _errorName);

	void SolarSystem(float _speed);

	bool showPlanets;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::ShaderProgram m_simpleShader;
	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;

	FlyCamera m_flyCamera;
	StationaryCamera m_stationaryCamera;
	OrbitalCamera m_orbitingCamera;

};