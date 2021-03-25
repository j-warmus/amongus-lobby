#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "Object.h"
#include "Cube.h"
#include "Model.h"
#include "Skybox.h"
#include "Sphere.h"
#include "Light.h"
#include "Node.h"
#include "Transform.h"
#include "Geometry.h"
#include "ParticleSystem.h"
#include <random>

class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	// Objects to Render
	//static Cube* cube;
	//static Model* bunnyPoints;
	//static Model* bearPoints;
	//static Model* sandalPoints;
	//static Sphere* sphere;
	//static Skybox* skybox;
	static Light* light;
	static Geometry* lobby;
	static Geometry* player;
	//static Transform* world;
	//static Transform* innerring2base;
	//static Transform* outerring2inner;
	//static Transform* cars;

	////controllers for movement
	//static float carpos;
	//static int cardir;
	//static int cont1, cont2, cont3, cont4;
	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eyePos, lookAtPoint, upVector;

	// Shader Program ID
	static GLuint shaderProgram;

	// Constructors and Destructors
	static bool initializeProgram();
	static bool loadShader(int type);
	static bool initializeObjects();
	static void cleanUp();

	//for scene graph:
	float rottracker;

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	
	// trackball mapping
	static glm::vec3 trackBallMapping(glm::vec2 point);
	// see if pc can move
	static bool validPos(glm::vec3 pos);
};

#endif