#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "Node.h"
#include <list>
#include <string>
#include <vector>
#include <random>

class Geometry : public Node
{
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> vertexIndicesVec;
	std::vector<glm::ivec3> normalIndicesVec;

	glm::vec3 ambient, diffuse, specular, color;
	glm::vec3 eyePos;
	float shininess;

	glm::mat4 model;
	glm::mat4 modelview;
	// glm::vec3 center;
	GLuint shader;
	Light* light;
	GLuint VAO, points_VBO, normals_VBO, EBO;
	float bsphere_radius;
	glm::vec3 location;
	glm::vec3 orientation;

	// spawning stuff
	float spawntime;
	float despawntime;
	bool spawned;
	bool moving;

public:
	Geometry(std::string objFilename, GLuint shaderProgram, Light* light_source, int load_mode);
	~Geometry();
	void draw_ext(const glm::mat4& view, const glm::mat4& projection, glm::vec3 viewPos, Light* light);
	void draw(glm::mat4 C);
	void set_material(glm::vec3 ambvec, glm::vec3 diffvec, glm::vec3 specvec, float shin, glm::vec3 col);
	void update_eyePos(glm::vec3 eye);
	void scale(float factor);
	void translate(glm::vec3 loc);
	void rotate(glm::vec3 axis, float deg, int mode);
	void update_modelview(glm::mat4 M) { modelview = M; }
	void update(glm::mat4 C) { return; }
	glm::vec3 get_loc() { return location; }
	void set_loc(glm::vec3 newloc);
	glm::vec3 get_orientation() { return orientation; }
	void set_orientation(glm::vec3 new_orientation);
	void update_astro();
	void move(float speed);
	float get_spawntime() { return spawntime; }
	float get_despawntime() { return despawntime; }
	bool is_moving() { return moving; }
	bool is_spawned() { return spawned; }
	void spawn_tog() { spawned = !spawned; }
	void move_tog() { moving = !moving; }
	float get_radius() { return bsphere_radius; }
};

#endif