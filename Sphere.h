#ifndef _SPHERE_H_
#define _SPHERE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sphere {
	public:
		GLuint shader;
		GLuint vao = 0, vbo = 0, vbo_n = 0, ebo = 0;
		glm::mat4 model;
		int stackCount = 30;
		int sectorCount = 30;
		int numsToDraw;
		GLuint skyboxTexture;
		Sphere();
		void draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& eye);
		void useShader(GLuint s) { shader = s; }
		void setSkybox(GLuint s) { skyboxTexture = s; }
};

#endif