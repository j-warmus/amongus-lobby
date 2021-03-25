#include "Window.h"
#include "Transform.h"
#include <time.h>

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Among Us . . . in 3D!??!?!?!?";

Geometry* Window::lobby;
Geometry* Window::player;
Light* Window::light;
std::vector<Geometry*> npc_astros;
std::vector<Geometry*> all_astros;
// if I did this project again I would implement these as classes, for now though this works
// x,y,z,r vectors for fixed bounding spheres
std::vector<glm::vec4> b_spheres;
// this is a vector of location :: normal pairs, meant to be iterated over in twos
std::vector < glm::vec3 > b_planes;

// particle system
float prev_time;
GLuint particle_shader;
std::vector<ParticleSystem*> particlesystems;

// light movement mode, 1 = standard, 2 move only light, 3 move both
int lightmode = 1;

// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

// View Matrix:
glm::vec3 Window::eyePos(0, 35, 30);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

// Shader Program ID
GLuint Window::shaderProgram;

// Rotate flag
bool movement_allowed;

// last known mouse position
glm::vec2 mousepos;
glm::vec3 lastPoint;

float rotScale = 100;
float rottracker = 0;

bool Window::loadShader(int type)
{
	if (type == 1) 
	{
		// Create a shader program with a vertex shader and a fragment shader.
		shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	}
	else if (type == 2) {
		// Create a shader program with a vertex shader and a fragment shader.
		shaderProgram = LoadShaders("shaders/skybox_shader.vert", "shaders/skybox_shader.frag");
	}
	// Check the shader program.

	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	// Check the shader program.
	return true;
}
bool Window::initializeProgram() {

	return true;
}

bool Window::initializeObjects()
{

	light = new Light(glm::vec3(1, 1, 1), glm::vec3(10, 20, 10), .08f);


	GLuint phong = LoadShaders("shaders/phong.vert", "shaders/phong.frag");
	GLuint toon = LoadShaders("shaders/toon.vert", "shaders/toon.frag");
	particle_shader = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// create lobby

	lobby = new Geometry("amongus_lobby.obj", phong, light, 1);
	lobby->set_material(glm::vec3(0.1, 0.1, 0.1), glm::vec3(.55, .55, .55), glm::vec3(.7, .7, .7), 0.8, glm::vec3(.8, .8, .8));
	
	// create player

	player = new Geometry("amongus_astro_still.obj", toon, light, 1);
	player->set_material(glm::vec3(0.1, 0.1, 0.1), glm::vec3(.6, .6, .6), glm::vec3(.9, .9, .9), 0.8, glm::vec3(0.7725490196078432, 0.07058823529411765, 0.06666666666666667));
	player->set_loc(glm::vec3(0, 2, 0));
	player->set_orientation(glm::vec3(0,0,1));
	player->update_astro();
	player->spawn_tog();

	// create dudes, add to 
	
	Geometry * black = new Geometry("amongus_astro_still.obj", toon, light, 1);
	npc_astros.push_back(black);
	Geometry * blue = new Geometry("amongus_astro_still.obj", toon, light, 1);
	npc_astros.push_back(blue);
	Geometry * brown = new Geometry("amongus_astro_still.obj", toon, light, 1);
	npc_astros.push_back(brown);
	Geometry * cyan = new Geometry("amongus_astro_still.obj", toon, light, 1);
	npc_astros.push_back(cyan);
	Geometry * green = new Geometry("amongus_astro_still.obj", toon, light, 1);
	npc_astros.push_back(green);
	Geometry * lime = new Geometry("amongus_astro_still.obj", toon, light, 1);
	npc_astros.push_back(lime);
	Geometry * orange = new Geometry("amongus_astro_still.obj", toon, light, 1);
	npc_astros.push_back(orange);
	Geometry * pink = new Geometry("amongus_astro_still.obj", toon, light, 1);
	npc_astros.push_back(pink);
	Geometry * yellow = new Geometry("amongus_astro_still.obj", toon, light, 1);
	npc_astros.push_back(yellow);

	// make a new vector with all astros for bounding purposes

	all_astros = npc_astros;
	all_astros.push_back(player);

	// create colors

	std::vector<glm::vec3> colors;
	glm::vec3 black_c = glm::vec3(0.24313725490196078, 0.2784313725490196, 0.3058823529411765);
	colors.push_back(black_c);
	glm::vec3 blue_c = glm::vec3(0.07450980392156863, 0.1803921568627451, 0.8196078431372549);
	colors.push_back(blue_c);
	glm::vec3 brown_c = glm::vec3(0.44313725490196076, 0.28627450980392155, 0.11372549019607843);
	colors.push_back(brown_c);
	glm::vec3 cyan_c = glm::vec3(0.2235294117647059, 0.996078431372549, 0.8666666666666667);
	colors.push_back(cyan_c);
	glm::vec3 green_c = glm::vec3(0.07450980392156863, 0.5019607843137255, 0.17254901960784313);
	colors.push_back(green_c);
	glm::vec3 lime_c = glm::vec3(0.3058823529411765, 0.9372549019607843, 0.2196078431372549);
	colors.push_back(lime_c);
	glm::vec3 orange_c = glm::vec3(0.9450980392156862, 0.49019607843137253, 0.047058823529411764);
	colors.push_back(orange_c);
	glm::vec3 pink_c = glm::vec3(0.9254901960784314, 0.32941176470588235, 0.7333333333333333);
	colors.push_back(pink_c);
	glm::vec3 yellow_c = glm::vec3(0.9647058823529412, 0.9647058823529412, 0.3411764705882353);
	colors.push_back(yellow_c);

	// set up bounding plane vec

	// left wall
	b_planes.push_back(glm::vec3(-16, 0, 0));
	b_planes.push_back(glm::vec3(1, 0, 0));
	// right wall
	b_planes.push_back(glm::vec3(16, 0, 0));
	b_planes.push_back(glm::vec3(-1, 0, 0));
	// top wall
	b_planes.push_back(glm::vec3(0, 0, -12));
	b_planes.push_back(glm::vec3(0, 0, 1));
	// bottom wall
	b_planes.push_back(glm::vec3(0, 0, 20));
	b_planes.push_back(glm::vec3(0, 0, -1));
	//botright
	b_planes.push_back(glm::vec3(14.4, 0, 19));
	b_planes.push_back(glm::normalize(glm::vec3(-1, 0, -1)));
	//botleft
	b_planes.push_back(glm::vec3(-14.4, 0, 19));
	b_planes.push_back(glm::normalize(glm::vec3(1, 0, -1)));
	//topright
	b_planes.push_back(glm::vec3(11.4, 0, -8.4));
	b_planes.push_back(glm::normalize(glm::vec3(-1, 0, 1)));
	//topleft
	b_planes.push_back(glm::vec3(-11.4, 0, -8.4));
	b_planes.push_back(glm::normalize(glm::vec3(1, 0, 1)));

	// set up crate bounding spheres
	b_spheres.push_back(glm::vec4(-7.5, 2, -0.3, 2.0));
	b_spheres.push_back(glm::vec4(-9.6, 2, 11.4, 2.5));
	b_spheres.push_back(glm::vec4(10.5, 2, 8.4, 2.5));

	// spawn npc astronauts
	Geometry* astro;
	// spawn in fixed location (to prevent spawning in walls etc)
	// spawn with random orientation
	srand(time(NULL));
	int j = 0;
	for (unsigned i = 0; i < npc_astros.size(); i++)
	{
		astro = npc_astros[i];
		astro->set_material(glm::vec3(0.1, 0.1, 0.1), glm::vec3(.6, .6, .6), glm::vec3(.9, .9, .9), 0.8, colors[i]);
		astro->set_loc(glm::vec3((i % 3) * 11.0 - 10.5, 2, (j % 3) * 10.0 - 4.0));
		// super lazy fix for lime spawning in crate
		if (i == 5) {
			astro->set_loc(glm::vec3((i % 3) * 10.0 - 11.0, 2, (j % 3) * 10.0 - 8.0));
		}
		int randx = rand() % 100 - 50;
		int randz = rand() % 100 - 50;

		astro->set_orientation(glm::vec3(randx, 0, randz));

		if (i % 3 == 2) { j++; }

		astro->update_astro();
		
	}

	prev_time = glfwGetTime();

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete lobby;
	delete player;
	delete light;
	Geometry* astro;
	for (unsigned i = 0; i < npc_astros.size(); i++) {
		astro = npc_astros[i];
		delete astro;
	}
	for (unsigned i = 0; i < particlesystems.size(); i++) {
		delete particlesystems[i];
	}
	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
								double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	// prune particle systems
	for (unsigned i = 0; i < particlesystems.size(); i++) {
		if (!particlesystems[i]->isalive()) {
			delete particlesystems[i];
			particlesystems.erase(particlesystems.begin() + i);
		}
	}


	Geometry* astro;
	for (unsigned i = 0; i < npc_astros.size(); i++)
	{
		// handle astro spawning/despawning

		astro = npc_astros[i];
		float time = glfwGetTime();
		if (time < astro->get_despawntime() && time > astro->get_spawntime() && astro->is_spawned() == false) {
			astro->spawn_tog();
			// activate particle effect
			particlesystems.push_back(new ParticleSystem(astro->get_loc(), particle_shader, glm::vec3(1.0, 1.0, 1.0)));
		}
		else if (glfwGetTime() > astro->get_despawntime() && astro->is_spawned() == true) {
			astro->spawn_tog();
			// activate particle effect
			particlesystems.push_back(new ParticleSystem(astro->get_loc(), particle_shader, glm::vec3(1, 0, 0)));
		}

		// randomly toggle move status, always start stationary
		// different odds to stop and start because it was kinda janky looking
		if (astro->is_spawned()) {
			int rand_num = rand();
			if (astro->is_moving() && rand_num % 14000 == 1) { astro->move_tog(); }
			else if (!astro->is_moving() && rand_num % 4000 == 1) { astro->move_tog(); }
		}
	}
	// update particles
	float cur_time = glfwGetTime();
	float t_delta = cur_time - prev_time;
	prev_time = cur_time;
	for (unsigned i = 0; i < particlesystems.size(); i++) {
		particlesystems[i]->update(t_delta);
	}


	float dist;
	// move astros/check for collisions
	for (unsigned i = 0; i < npc_astros.size(); i++)
	{
		astro = npc_astros[i];
		if (astro->is_spawned() && astro->is_moving())
		{ 
			// check for plane collisions
			for (unsigned j = 0; j < b_planes.size(); j = j + 2) 
			{
				glm::vec3 p = b_planes[j];
				glm::vec3 n = b_planes[j + 1];

				dist = glm::dot((astro->get_loc() - p), n);
				if (dist < astro->get_radius()) {
					glm::vec3 new_orientation;
					new_orientation = glm::reflect(astro->get_orientation(), n);
					astro->set_orientation(new_orientation);
					// give a push so they don't get stuck
					astro->set_loc(astro->get_loc() + n * 0.01f);
				}
			}
			// check for box collisions
			for (unsigned k = 0; k < b_spheres.size(); k++)
			{
				glm::vec3 sphere;
				sphere.x = b_spheres[k].x;
				sphere.y = b_spheres[k].y;
				sphere.z = b_spheres[k].z;
				float radius = b_spheres[k].w;
				dist = glm::abs(glm::distance(sphere, astro->get_loc()));
				if (dist < astro->get_radius() + radius) {
					glm::vec3 new_orientation;
					new_orientation = glm::reflect(astro->get_orientation(), sphere - astro->get_loc());
					astro->set_orientation(new_orientation);
				}
			}

			// check for astro collisions
			for (unsigned l = 0; l < all_astros.size(); l++)
			{
				Geometry* other_astro = all_astros[l];
				if (other_astro == astro) { continue; }
				else if (!other_astro->is_spawned()) { continue; }

				dist = glm::abs(glm::distance(other_astro->get_loc(), astro->get_loc()));
				if (dist < astro->get_radius() + other_astro->get_radius()) {
					glm::vec3 new_orientation;
					new_orientation = glm::reflect(astro->get_orientation(), other_astro->get_loc() - astro->get_loc());
					astro->set_orientation(new_orientation);
					// give a push so they don't get stuck
					glm::vec3 a = glm::normalize(astro->get_loc() - other_astro->get_loc());
					astro->set_loc(other_astro->get_loc() + 1.001f*a*(astro->get_radius() + other_astro->get_radius()));
				}
			}

			astro->move(0.005f);


		}
		
	}

}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


	lobby->draw_ext(view, projection, eyePos, light);
	player->draw_ext(view, projection, eyePos, light);

	for (unsigned i = 0; i < particlesystems.size(); i++) {
		particlesystems[i]->draw(view, projection);
	}

	//printf("%f, %f\n", player->get_spawntime(), player->get_despawntime());
	Geometry* astro;
	for (unsigned i = 0; i < npc_astros.size(); i++)
	{
		astro = npc_astros[i];
		if (astro->is_spawned()){ astro->draw_ext(view, projection, eyePos, light); }
	}

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	// Check for a key press.
	if (action == GLFW_REPEAT || action == GLFW_PRESS)
	{
		glm::mat4 trans;
		glm::mat4 invtrans;
		glm::vec4 temp;

		float movespeed = 0.3;
		float rotspeed = 12.0f;
		switch (key)
		{
			//New camera controls
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_A:
			if (validPos(player->get_loc() + glm::vec3(-1.0, 0.0, 0.0) * movespeed)) {
				player->set_orientation(glm::vec3(-1.0, 0.0, 0.0));
				player->move(movespeed);
			}

			break;
		case GLFW_KEY_D:
			if (validPos(player->get_loc() + glm::vec3(1.0, 0.0, 0.0) * movespeed)) {
				player->set_orientation(glm::vec3(1.0, 0.0, 0.0));
				player->move(movespeed);
			}
			break;
		case GLFW_KEY_W:
			if (validPos(player->get_loc() + glm::vec3(0.0, 0.0, -1.0) * movespeed)) {
				player->set_orientation(glm::vec3(0.0, 0.0, -1.0));
				player->move(movespeed);
			}
			break;
		case GLFW_KEY_S:
			if (validPos(player->get_loc() + glm::vec3(0.0, 0.0, 1.0) * movespeed)) {
				player->set_orientation(glm::vec3(0.0, 0.0, 1.0));
				player->move(movespeed);
			}
			break;
		}
		// printf("%f,%f,%f\n", player->get_loc().x, player->get_loc().y, player->get_loc().z);
	}
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0) {
		eyePos = eyePos - (eyePos - lookAtPoint) / 100.0f;
		view = glm::lookAt(eyePos, lookAtPoint, upVector);
	}
	else if (yoffset < 0) {
		eyePos = eyePos + (eyePos - lookAtPoint) / 100.0f;
		view = glm::lookAt(eyePos, lookAtPoint, upVector);
	}
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		movement_allowed = true;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		mousepos.x = xpos; mousepos.y = ypos;
		lastPoint = Window::trackBallMapping(mousepos);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		movement_allowed = false;
	}
}

void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	glm::vec3 direction, curPoint;
	float pixel_diff, rot_angle;
	glm::vec2 curPos;

	curPos.x = xpos; curPos.y = ypos;
	if (movement_allowed) {
		curPoint = trackBallMapping(curPos);
		direction = curPoint - lastPoint;
		float velocity = glm::length(direction);
		if (velocity > 0.0001) {
			glm::vec3 rotAxis;
			rotAxis = glm::cross(lastPoint, curPoint);
			rot_angle = velocity * rotScale;
			rotAxis = glm::vec3(rotAxis[0], 0, 0);
			if (rotAxis[0] == 0){rotAxis[0] = 0.000000001; }
			eyePos = glm::vec3(glm::rotate(glm::radians(rot_angle), rotAxis) * glm::vec4(eyePos,1.0));
			view = glm::lookAt(eyePos, lookAtPoint, upVector);
		}
		lastPoint = curPoint;

	}
}

glm::vec3 Window::trackBallMapping(glm::vec2 point)
{
	glm::vec3 v;
	double d;
	v.x = (2.0 * point.x - width) / width;
	v.y = (height - 2.0 * point.y) / height;
	v.z = 0.0;
	d = glm::length(v);
	d = (d < 1.0) ? d : 1.0;
	v.z = sqrtf(1.001 - d * d);
	glm::normalize(v);
	return v;
}

bool Window::validPos(glm::vec3 pos) {
	bool valid = true;

	float dist;
	// check for plane collisions
	for (unsigned j = 0; j < b_planes.size(); j = j + 2)
	{
		glm::vec3 p = b_planes[j];
		glm::vec3 n = b_planes[j + 1];

		dist = glm::dot((pos - p), n);
		if (dist < player->get_radius()) {
			valid = false;
		}
	}
	// check for box collisions
	for (unsigned k = 0; k < b_spheres.size(); k++)
	{
		glm::vec3 sphere;
		sphere.x = b_spheres[k].x;
		sphere.y = b_spheres[k].y;
		sphere.z = b_spheres[k].z;
		float radius = b_spheres[k].w;
		dist = glm::abs(glm::distance(sphere, pos));
		if (dist < player->get_radius() + radius) {
			valid = false;
		}
	}

	// check for astro collisions
	for (unsigned l = 0; l < npc_astros.size(); l++)
	{
		Geometry* other_astro = npc_astros[l];
		if (!other_astro->is_spawned()) { continue; }
		dist = glm::abs(glm::distance(other_astro->get_loc(), pos));
		if (dist < player->get_radius() + other_astro->get_radius()) {
			valid = false;
		}
	}
	return valid;
}