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

	// ind the shader
	m_simpleShader.bind();

	m_simpleShader.bindUniform("ProjectionViewModel", pv * m_quadTransform);

	m_quadMesh.Draw();

	Gizmos::draw(pv);
}

void GraphicsApp::ImGUI_Helper()
{
	ImGui::Begin("Planet Loader");

	ImGui::Text("Planet Display");
	ImGui::Checkbox("Show Planets", &showPlanets);

	ImGui::End();
}

bool GraphicsApp::LaunchShaders()
{
	if (!LoadShaders(m_simpleShader, "./shaders/simple.", "Simple Shader"))
		return false;

	m_quadMesh.InitialiseQuad();

	m_quadTransform = {
		10,0,0,0,
		0,10,0,0,
		0,0,10,0,
		0,0,0,1
	};

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
		printf(_errorName.append(" Shader Error: %s\n").c_str(), _shaderToLoad.getLastError());
		return false;
	}

	return true;
}

void GraphicsApp::SolarSystem(float _speed)
{
	Planet* sun = new Planet(glm::vec3(0), 1, glm::vec4(0.8f, 0.8f, 0, 1));
	Planet* mercury = new Planet(glm::vec3(1, 0, 0), .1f, glm::vec4(0.2f, 0.2f, 0.2f, 1), sun);
	Planet* venus = new Planet(glm::vec3(2, 0, 0), .1f, glm::vec4(0.5f, 0.5f, 0, 1), sun);
	Planet* earth = new Planet(glm::vec3(3, 0, 0), .1f, glm::vec4(0, 0, 1, 1), sun);
	Planet* mars = new Planet(glm::vec3(4, 0, 0), .1f, glm::vec4(0.3f, 0.3f, 0, 1), sun);
	Planet* Jupiter = new Planet(glm::vec3(5, 0, 0), .5f, glm::vec4(0.3, 0.3, 0, 1), sun);
	Planet* Saturn = new Planet(glm::vec3(6, 0, 0), .5f, glm::vec4(0.3, 0.3, 1, 1), sun);
	Planet* Uranus = new Planet(glm::vec3(7, 0, 0), .3f, glm::vec4(0.2, 0.2, .8f, 1), sun);
	Planet* Nerptune = new Planet(glm::vec3(8, 0, 0), .3f, glm::vec4(0, 0, 1, 1), sun);
	Planet* pluto = new Planet(glm::vec3(9, 0, 0), .1f, glm::vec4(0, 0, 1, 1), sun);

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
