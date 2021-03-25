#ifndef _MODEL_H_
#define _MODEL_H_

#include "Object.h"
#include "Light.h"
#include <vector>
#include <string>

using namespace std;

class Model : public Object
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> pointIndices;
	std::vector<glm::ivec3> normalIndices;

	glm::vec3 ambient, diffuse, specular, color;
	float shininess;
	glm::vec3 center;
	GLuint VAO, points_VBO, normals_VBO, EBO;
	GLfloat pointSize;
	//int mode;

public:
	Model(std::string objFilename, GLfloat pointSize);
	~Model();
	GLuint shader;
	void useshader(GLuint s) { shader = s; }
	void draw(const glm::mat4& view, const glm::mat4& projection, glm::vec3 viewPos, Light * light);
	
	void update();

	void updatePointSize(GLfloat size);
	void spin(float deg);
	void scale(float factor);
	void translate(glm::vec3 loc);
	void rotate(glm::vec3 axis, float deg, int mode);
	// swap_mode();
	void set_material(glm::vec3 ambvec, glm::vec3 diffvec, glm::vec3 specvec, float shin, glm::vec3 col);
	glm::vec3 return_center();
};

#endif
