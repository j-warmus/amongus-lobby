#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <string>
//#include <vector>

class Object
{
protected:
	glm::mat4 model;
	glm::vec3 color;

public:
	glm::mat4 getModel() { return model; }
	glm::vec3 getColor() { return color; }

//	virtual void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader, glm::vec3 viewPos, glm::vec3 lightcolor, glm::vec3 lightpos, float lightattenuation);
	virtual void update() = 0;
	virtual void updatePointSize(GLfloat size) = 0;
	virtual void spin(float deg) = 0;
	virtual void scale(float factor) = 0;
//	virtual void rotate(glm::vec3 axis, float deg) = 0;
};

#endif

