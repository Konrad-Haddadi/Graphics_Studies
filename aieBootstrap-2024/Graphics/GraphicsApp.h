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
#include "Texture.h"
#include "OBJMesh.h"

class GraphicsApp : public aie::Application {
public:

	GraphicsApp();
	virtual ~GraphicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();
	void ImGUI_Helper();
	void SpawnCube();
	void SpawnSquare();
	void SpawnCylinder(float _radius, float _height, int _segments);


protected:

	bool LaunchShaders();
	bool LoadShaders(aie::ShaderProgram& _shaderToLoad, const char* _filePath, std::string _errorName);

	bool ObjLoader(aie::OBJMesh& __objMesh, glm::mat4& _transform, const char* _filepath, std::string _filename = " ", bool _flipTextures = false, float _scale = 1.f, glm::vec3 _position = { 0,0,0 });

	void SolarSystem(float _speed);

	bool showPlanets;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_simpleTexture;

	// Simple Mesh
	Mesh m_quadMesh;
	Mesh m_boxMesh;

	aie::Texture m_gridTexture;

	// Spear mesh
	aie::OBJMesh m_spearMesh;
	glm::mat4 m_spearTransform;

	// Bunny Mesh
	aie::OBJMesh m_bunnyMesh;
	glm::mat4 m_bunnyTransform;

	// Super Meat Boy
	aie::OBJMesh m_meatBoyMesh;
	glm::mat4 m_meatBoyTransform;
	aie::ShaderProgram m_meatBoyTexture;


	glm::mat4 m_quadTransform;
	glm::mat4 m_boxTransform;

	FlyCamera m_flyCamera;
	StationaryCamera m_stationaryCamera;
	OrbitalCamera m_orbitingCamera;

};