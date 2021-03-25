#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "Object.h"
#include <vector>
#include <string>

#include <iostream>

using namespace std;

class Skybox
{
private:
	GLuint shader;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	unsigned int skyboxTexture;
	

public:
	Skybox();
	
	~Skybox();
	static unsigned int loadCubemap(std::vector<std::string> faces);
	void draw(const glm::mat4& view, const glm::mat4& projection);
	void useShader(GLuint s) { shader = s; }
	GLuint getSkybox() { return skyboxTexture; }
};

#endif