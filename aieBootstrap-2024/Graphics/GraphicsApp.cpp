#include "GraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include "Planet.h"
#include "FlyCamera.h"
#include "BaseCamera.h"

#include <imgui_glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <list>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

std::list <Planet*> solarSystem;

BaseCamera* bc = nullptr;

GraphicsApp::GraphicsApp() 
{
}

GraphicsApp::~GraphicsApp() {

}

bool GraphicsApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	SolarSystem(1);

	showPlanets = false;

	return LaunchShaders();
}

void GraphicsApp::shutdown() {

	Gizmos::destroy();
}

void GraphicsApp::update(float deltaTime) {

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));
	bc = &m_flyCamera;

	bc->CalculationUpdate(deltaTime);

	// custom function
	if (showPlanets)
	{
		for each (Planet * planet in solarSystem)
			planet->Update(deltaTime);
	}

	

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	ImGUI_Helper();
}

void GraphicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// custom function

	if (showPlanets)
	{
		for each (Planet * planet in solarSystem)
			planet->Draw();
	}

	// update perspective based on screen size
	// m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	m_viewMatrix = bc->GetViewMatrix();
	m_projectionMatrix = bc->GetProjectionMatrix();

	auto pv = m_projectionMatrix * m_viewMatrix;

	m_meatBoyTexture.bind();
	m_meatBoyTexture.bindUniform("ProjectionViewModel", pv * m_meatBoyTransform);
	m_meatBoyMesh.draw();

	Gizmos::draw(pv);
}

void GraphicsApp::ImGUI_Helper()
{
	ImGui::Begin("Planet Loader");

	ImGui::Text("Planet Display");
	ImGui::Checkbox("Show Planets", &showPlanets);

	ImGui::End();
}

void GraphicsApp::SpawnCube()
{
	// define 4 vertices for 2 triangles
	Mesh::Vertex vertices[8];

	// Top
	vertices[0].position = { -0.5f, 0.5f, 0.5f, 1 };
	vertices[1].position = { 0.5f,  0.5f, 0.5f, 1 }; 
	vertices[2].position = { -0.5f, 0.5f, -0.5f, 1 }; 
	vertices[3].position = { 0.5f,  0.5f, -0.5f, 1 }; 

	// Base
	vertices[4].position = { -0.5f, -0.5f, 0.5f, 1 };	// front right
	vertices[5].position = { 0.5f, -0.5f, 0.5f, 1 };	// back right
	vertices[6].position = { -0.5f, -0.5f, -0.5f, 1 }; // front left 
	vertices[7].position = { 0.5f, -0.5f, -0.5f, 1 };	// back left

	unsigned int indices[38] = { 2, 3, 1, 1, 0, 2,
								  7, 5, 1, 1, 3, 7,
								  6, 7, 3, 3, 2, 6,
								  5, 4, 0, 0, 1, 5,
								  4, 5, 6, 6, 5, 7 };

	m_boxMesh.Initialise(8, vertices, 38, indices);

	for (int i = 0; i < 8; i++)
	{
		if(i < 7)
			Gizmos::addLine(vertices[i].position, vertices[i + 1].position, glm::vec4(1, 1, 1, 1));
		else	
			Gizmos::addLine(vertices[i].position, vertices[0].position, glm::vec4(1, 1, 1, 1));
	}
}

void GraphicsApp::SpawnSquare()
{
	//// define 4 vertices for 2 triangles
	//Mesh::Vertex vertices[4];

	//// Base
	//vertices[0].position = { -0.5f, 0, 0.5f, 1 };	// front right
	//vertices[1].position = { 0.5f, 0, 0.5f, 1 };	// back right
	//vertices[2].position = { -0.5f, 0, -0.5f, 1 }; // front left 
	//vertices[3].position = { 0.5f, 0, -0.5f, 1 };	// back left

	//unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };

	//m_quadMesh.Initialise(4, vertices, 6, indices);
}

void GraphicsApp::SpawnCylinder(float _radius, float _height, int _segments)
{
	// define 4 vertices for 2 triangles

	if (_segments > 30)
	{
		printf("Segment amount is greater than 30");
		return;
	}


	Mesh::Vertex vertices[30];

	// Base
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };	// front right
	vertices[2].position = { 0.5f, 0, 0.5f, 1 };	// back right
	vertices[4].position = { -0.5f, 0, -0.5f, 1 }; // front left 
	vertices[6].position = { 0.5f, 0, -0.5f, 1 };	// back left



	//m_boxMesh.Initialise(8, vertices, 38, indices);
}

bool GraphicsApp::LaunchShaders()
{
	if (!LoadShaders(m_meatBoyTexture, "./shaders/textured.", "Textured Shader"))
		return false;

	ObjLoader(m_meatBoyMesh, m_meatBoyTransform, "./super_meatboy/Super_meatboy.obj", "MeatBoy", true);
	
	return true;
}

bool GraphicsApp::LoadShaders(aie::ShaderProgram& _shaderToLoad, const char* _filePath, std::string _errorName)
{
	std::string vert = _filePath;
	std::string frag = _filePath;

	_shaderToLoad.loadShader(aie::eShaderStage::VERTEX, vert.append("vert").c_str());
	_shaderToLoad.loadShader(aie::eShaderStage::FRAGMENT, frag.append("frag").c_str());

	if (_shaderToLoad.link() == false)
	{
		printf(_errorName.append("Shader Error: %s\n").c_str(), _shaderToLoad.getLastError());
		return false;
	}

	return true;
}

bool GraphicsApp::ObjLoader(aie::OBJMesh& __objMesh, glm::mat4& _transform, const char* _filepath, std::string _filename, bool _flipTextures, float _scale, glm::vec3 _position) 
{
	if (__objMesh.load(_filepath, true, _flipTextures) == false)
	{
		printf("Object Mesh loading had an error");
		return false;
	}

	_transform = {
		_scale, 0,0,0,
		0, _scale, 0,0,
		0,0,_scale, 0,
		_position.x, _position.y, _position.z, 1
	};

	return false;
}

void GraphicsApp::SolarSystem(float _speed)
{
	Planet* sun = new Planet(glm::vec3(0), 1, glm::vec4(0.8f, 0.8f, 0, 1));
	Planet* mercury = new Planet(glm::vec3(1, 0, 0), .1f, glm::vec4(0.2f, 0.2f, 0.2f, 1), sun, 1);
	Planet* venus = new Planet(glm::vec3(2, 0, 0), .1f, glm::vec4(0.5f, 0.5f, 0, 1), sun, .5f);
	Planet* earth = new Planet(glm::vec3(3, 0, 0), .1f, glm::vec4(0, 0, 1, 1), sun, 2);
	Planet* mars = new Planet(glm::vec3(4, 0, 0), .1f, glm::vec4(0.3f, 0.3f, 0, 1), sun, 5);
	Planet* Jupiter = new Planet(glm::vec3(5, 0, 0), .5f, glm::vec4(0.3, 0.3, 0, 1), sun, 3);
	Planet* Saturn = new Planet(glm::vec3(6, 0, 0), .5f, glm::vec4(0.3, 0.3, 1, 1), sun, 15);
	Planet* Uranus = new Planet(glm::vec3(7, 0, 0), .3f, glm::vec4(0.2, 0.2, .8f, 1), sun,.1f);
	Planet* Nerptune = new Planet(glm::vec3(8, 0, 0), .3f, glm::vec4(0, 0, 1, 1), sun, 5);
	Planet* pluto = new Planet(glm::vec3(9, 0, 0), .1f, glm::vec4(0, 0, 1, 1), sun, 17);

	m_orbitingCamera.SetTarget(sun->position, 5);

	solarSystem.push_back(sun);
	solarSystem.push_back(mercury);
	solarSystem.push_back(venus);
	solarSystem.push_back(earth);
	solarSystem.push_back(mars);
	solarSystem.push_back(Jupiter);
	solarSystem.push_back(Saturn);
	solarSystem.push_back(Uranus);
	solarSystem.push_back(Nerptune);
	solarSystem.push_back(pluto);
}
