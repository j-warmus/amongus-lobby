#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(glm::vec3 position, GLuint shader_program, glm::vec3 col)
{
	model = glm::mat4(1);

	color = col;
	alive = true;
	origin = position;
	shader = shader_program;
	for (unsigned i = 0; i < MAX_PARTICLES; i++) {
		position_data.push_back(position);
		particles.push_back(Particle(position, 3.0f));
	}
	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * position_data.size(), position_data.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void ParticleSystem::update(float timeDelta)
{
	if (alive) {
		for (unsigned i = 0; i < particles.size(); i++) {
			particles[i].update(timeDelta);
			position_data[i] = particles[i].position;
			if (particles[i].life_left < 0.0f) {
				particles.erase(particles.begin() + i);
				position_data.erase(position_data.begin() + i);
			}
		}

		if (position_data.size() == 0) { alive = false; }
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * position_data.size(), position_data.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void ParticleSystem::draw(const glm::mat4& view, const glm::mat4& projection)
{
	if (alive) {
		// Actiavte the shader program 
		glUseProgram(shader);

		// Get the shader variable locations and send the uniform data to the shader 
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

		// Bind the VAO
		glBindVertexArray(VAO);
		float pointSize = 1.0f;
		// Set point size
		glPointSize(pointSize);

		// Draw the points 
		glDrawArrays(GL_POINTS, 0, position_data.size());

		// Unbind the VAO and shader program
		glBindVertexArray(0);
		glUseProgram(0);
	}
}

ParticleSystem::Particle::Particle(glm::vec3 origin, float life)
{
	position = origin;
	life_left = life;
	velocity = glm::vec3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50);
}

void ParticleSystem::Particle::update(float timeDelta)
{
	life_left -= timeDelta;
	position = position + velocity * timeDelta * 0.1f;
	velocity = velocity * 0.999f;
}