#include "GraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include "Planet.h"
#include "FlyCamera.h"
#include "BaseCamera.h"
#include "Lights.h"

#include <imgui_glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <list>
#include "Scene.h"
#include "Instance.h"
#include <gl_core_4_4.h>
#include <iostream>
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

	float aspecitRatio = getWindowWidth() / (float)getWindowHeight();

	m_flyCamera.SetAspectRatio(getWindowWidth(), getWindowHeight());
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, aspecitRatio, 0.1f, 1000.0f);

	m_flyCamera.SetProjectionMatrix(0.25f, aspecitRatio, 0.1f, 1000.0f);

	SolarSystem(1);

	showPlanets = false;

	bc = &m_flyCamera;

	Lights globalLight;

	globalLight.color = { 1,1,1 };
	globalLight.direction = { 1, 1, 1 };
	globalLight.intensity = 10;

	m_scene = new Scene(&m_flyCamera, glm::vec2(getWindowWidth(), getWindowHeight()), globalLight);	
	return LaunchShaders();
}

void GraphicsApp::shutdown() {

	Gizmos::destroy();
	delete m_scene;
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

	m_scene->Update(deltaTime);
	m_scene->ImGUI_Functions((float)getWindowWidth(), (float)getWindowHeight());
	
	m_scanlineTimer += deltaTime * deltaTime;
	ImGUI_Helper();

	float aspecitRatio = getWindowWidth() / (float)getWindowHeight();
	
	
	if (windowWidth != getWindowWidth())
	{
		m_renderTarget.initialise(1, getWindowWidth(), (float)getWindowHeight());		
	}

	windowWidth = getWindowWidth();	
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));

	m_flyCamera.SetAspectRatio(getWindowWidth(), (float)getWindowHeight());
	m_flyCamera.SetProjectionMatrix(0.25f, aspecitRatio, 0.1f, 1000.0f);

	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, aspecitRatio, 0.1f, 1000.0f);


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

}

void GraphicsApp::draw() {

	// Bind to the render target
	m_renderTarget.bind();

	// wipe the screen to the background colour
	clearScreen();

	// custom function

	if (showPlanets)
	{
		for each (Planet * planet in solarSystem)
			planet->Draw();
	}

	// update perspective based on screen size
	m_viewMatrix = bc->GetViewMatrix(); 
	m_projectionMatrix = bc->GetProjectionMatrix();
	m_scene->Draw();

	auto pv = m_projectionMatrix * m_viewMatrix;	 

	Gizmos::draw(pv);

	// Unbind the target from the backbuffer
	m_renderTarget.unbind();

	// Clear the back buffer
	clearScreen();
	
	float screenHeight = (float)getWindowHeight();

	m_postProcess.bind();
	m_postProcess.bindUniform("colorTarget", 0);
	m_postProcess.bindUniform("postProcessTarget", m_scene->GetPostProcess());
	m_postProcess.bindUniform("screenPos", m_scanlineTimer);
	m_postProcess.bindUniform("difference", m_scene->GetEdgeDetection());
	m_postProcess.bindUniform("pixels", m_scene->GetPixelAmount());
	m_postProcess.bindUniform("colorDifference", m_scene->GetColorDif());
	m_postProcess.bindUniform("whiteColor", m_scene->GetWhiteColorDif());
		

	m_renderTarget.getTarget(0).bind(0);

	m_screenQuad.Draw();	

}

void GraphicsApp::ImGUI_Helper()
{
		
}

bool GraphicsApp::LaunchShaders()
{
	// Loading Shaders
	if (!LoadShaders(m_boundTexture, "./shaders/textured.", "Textured", false))
		return false;

	if (!LoadShaders(m_normalMapPhong, "./shaders/normalMap.", "Textured and Normal Shader"))
		return false;

	if (!LoadShaders(m_color, "./shaders/color.", "Color", false))
		return false;

	if (!LoadShaders(m_postProcess, "./shaders/post.", "Post Processing", false))
		return false;


	if (m_renderTarget.initialise(1, getWindowWidth(), (float)getWindowHeight()) == false)
	{
		printf("Render Target has an error!!\n");
		return false;
	}

	m_quadMesh.InitialiseQuad();
	m_screenQuad.InitialiseFullScreenQuad();

	m_quadTransform = {
		10,0,0,0,
		0,10,0,0,
		0,0,10,0,
		0,0,0,1
	};

	// Load Mesh using Transform
	ObjLoader(m_spearMesh, m_spearTransform, "./soulspear/soulspear.obj", "Spear", true); 	
	//ObjLoader(m_meatBoyMesh, m_meatBoyTransform, "./super_meatboy/Super_meatboy.obj", "MeatBoy", true);

	return true;
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
	// define 4 vertices for 2 triangles
	Mesh::Vertex vertices[4];

	// Base
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };	// front right
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };	// back right
	vertices[2].position = { -0.5f, 0, -0.5f, 1 }; // front left 
	vertices[3].position = { 0.5f, 0, -0.5f, 1 };	// back left

	unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };

	m_phongMesh.Initialise(4, vertices, 6, indices);
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

bool GraphicsApp::LoadShaders(aie::ShaderProgram& _shaderToLoad, const char* _filePath, std::string _errorName, bool _addToScene)
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

	if(_addToScene)
		m_scene->AddShader(&_shaderToLoad, _errorName);

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

	m_scene->AddMesh(&__objMesh, _filename);

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
