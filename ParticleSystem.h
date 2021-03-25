#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "Node.h"
#include <list>
#include <string>
#include <vector>
#include <random>

class ParticleSystem
{
private:
	struct Particle {
		glm::vec3 position, velocity;
		float life_left;
		Particle(glm::vec3 origin, float life);
		void update(float timeDelta);
	};

	static const int MAX_PARTICLES = 200;
	static const int PARTICLE_LIFE = 3;
	bool alive;
	GLuint VAO, VBO;
	glm::vec3 origin;
	std::vector <Particle> particles;
	std::vector <glm::vec3> position_data;
	GLuint shader;
	glm::mat4 model;
	glm::vec3 color;
public:
	ParticleSystem(glm::vec3 position, GLuint shader_program, glm::vec3 col);
	~ParticleSystem();

	void update(float timeDelta);
	void draw(const glm::mat4& view, const glm::mat4& projection);
	bool isalive() { return alive; }
};

#endif