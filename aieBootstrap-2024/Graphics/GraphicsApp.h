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

class Scene;

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
	bool LoadShaders(aie::ShaderProgram& _shaderToLoad, const char* _filePath, std::string _errorName, int _addToScene = 0);

	bool ComplexObjLoader(aie::OBJMesh& __objMesh, glm::mat4& _transform, const char* _filepath, std::string _filename = " ", bool _flipTextures = false, float _scale = 1.f, glm::vec3 _position = { 0,0,0 });
	bool SimpleObjLoader(Mesh& __objMesh, glm::mat4& _transform, std::string _filename = " ");

	void SolarSystem(float _speed);

	bool showPlanets;
	Scene* m_scene;
	float m_scanlineTimer = 100;
	float m_timer = 0;
	
	float windowWidth;

	aie::RenderTarget m_renderTarget;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_color;
	aie::ShaderProgram m_simplePhong;
	aie::ShaderProgram m_classicPhong;
	aie::ShaderProgram m_texturedPhong;
	aie::ShaderProgram m_normalMapPhong;
	aie::ShaderProgram m_boundTexture;
	aie::ShaderProgram m_postProcess;

	struct BasicLight {
		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	BasicLight m_light;
	float m_diffuseMultiplier;
	float m_directionMultiplier;
	float m_specularMultiplier;

	glm::vec3 m_ambientLight;

	// Simple Mesh
	Mesh m_quadMesh;
	Mesh m_boxMesh;
	Mesh m_phongMesh;
	Mesh m_screenQuad;
	aie::Texture m_gridTexture;

	// Spear mesh
	aie::OBJMesh m_spearMesh;
	glm::mat4 m_spearTransform;
	aie::Texture m_spearTexture;

	// City
	aie::OBJMesh m_cityMesh;
	glm::mat4 m_cityTransform;
	aie::Texture m_cityTexture;

	// Bunny Mesh
	aie::OBJMesh m_bunnyMesh;
	glm::mat4 m_simpleTransform;

	// Super Meat Boy
	aie::OBJMesh m_meatBoyMesh;
	glm::mat4 m_meatBoyTransform;
	aie::ShaderProgram m_meatBoyShader;
	aie::Texture m_meatBoyTexture;

	// Dragon Mesh
	aie::OBJMesh m_dragonMesh;
	glm::mat4 m_dragonTransform;

	// Nomral Map Phong

	glm::mat4 m_quadTransform;
	glm::mat4 m_boxTransform;

	FlyCamera m_flyCamera;
	StationaryCamera m_stationaryCamera;
	OrbitalCamera m_orbitingCamera;
};