#ifndef _LIGHT_H_
#define _LIGHT_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light
{
	public:
		glm::vec3 color;
		glm::vec3 position;
		float attenuation;

		Light(glm::vec3 col, glm::vec3 pos, float atten);

		glm::vec3 getcolor() { return color; }
		glm::vec3 getpos() { return position; }
		float getatten() { return attenuation; }

		void setcolor(glm::vec3 c) { color = c; }
		void setpos(glm::vec3 p) { position = p; }
		void setatten(float a) { attenuation = a; }
		
};

#endif