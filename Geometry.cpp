#include "Geometry.h"
#include "sstream"
#include "fstream"
#include "iostream"

Geometry::Geometry(std::string objFilename, GLuint shaderProgram, Light* light_source, int load_mode)
{
	// LOAD MODE 1 IF THAT DOESNT WORK LOAD MODE 0
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	light = light_source;
	shader = shaderProgram;
	model = glm::mat4(1.0f);

	std::ifstream objFile(objFilename); // The obj file we are reading.

	// set spawn and despawn
	spawned = false;
	moving = false;
	spawntime = (rand() % 1500) / 100.0 + 1.0;
	despawntime = ((rand() % 3000) / 100.0) + spawntime + 10.0;

	orientation = glm::vec3(0.0, 0.0, 1.0);
	location = glm::vec3(0.0, 0.0, 0.0);
	bsphere_radius = 1.0f;

	// Check whether the file can be opened.
	if (objFile.is_open())
	{
		std::string line; // A line in the file.

		// Read lines from the file.
		while (std::getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			std::stringstream ss;
			ss << line;

			// Read the first word of the line.
			std::string label;
			ss >> label;

			if (label == "v")
			{

				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;
				temp_vertices.push_back(point);
			}
			else if (label == "vn") {
				glm::vec3 normal;
				ss >> normal.x >> normal.y >> normal.z;
				glm::normalize(normal);
				temp_normals.push_back(normal);

				//load point and no
			}
			else if (label == "f") {
				int a;
				std::string s_x, s_y, s_z;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				ss >> s_x >> s_y >> s_z;
				a = sscanf(s_x.c_str(), "%u/%u/%u", &vertexIndex[0], &uvIndex[0], &normalIndex[0]);
				a = sscanf(s_y.c_str(), "%u/%u/%u", &vertexIndex[1], &uvIndex[1], &normalIndex[1]);
				a = sscanf(s_z.c_str(), "%u/%u/%u", &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

				for (int i = 0; i < 3; i ++){
					vertexIndices.push_back(vertexIndex[i] - 1);
					uvIndices.push_back(uvIndex[i] - 1);
					normalIndices.push_back(normalIndex[i] - 1);
				}

				if (load_mode == 0) {
					glm::ivec3 vIdx, nIdx;
					vIdx.x = vertexIndex[0] - 1;
					vIdx.y = vertexIndex[1] - 1;
					vIdx.z = vertexIndex[2] - 1;
					vertexIndicesVec.push_back(vIdx);

				}

			}
		}
		
		if (load_mode == 1) {
			glm::ivec3 temp;

			for (unsigned i = 0; i < vertexIndices.size(); i++)
			{

				vertices.push_back(temp_vertices[vertexIndices[i]]);

				// uvs.push_back(input_uvs[normal_indices_[i]]);

				normals.push_back(temp_normals[normalIndices[i]]);

				int k = i % 3;

				if (k == 2) {
					temp.z = i;
					vertexIndicesVec.push_back(temp);
				}
				else {
					temp[k] = i;
				}
			}
		}
		else {
			vertices = temp_vertices;
			normals = temp_normals;
		}
	}
	else
	{
		std::cerr << "Can't open the file " << objFilename << std::endl;
	}
	objFile.close();




	// Generate a vertex array (VAO) and vertex buffer object (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &points_VBO);
	glGenBuffers(1, &normals_VBO);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass the vertex data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind VBO to the bound VAO, and store the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass the vertex data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * vertexIndicesVec.size(), vertexIndicesVec.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Geometry::~Geometry()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &points_VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
}

void Geometry::draw_ext(const glm::mat4& view, const glm::mat4& projection, glm::vec3 viewPos, Light* ext_light) 
{
	// Actiavte the shader program 
	glUseProgram(shader);
	glm::mat4 modelview2 = view * projection;
	glUniformMatrix4fv(glGetUniformLocation(shader, "cmatrix"), 1, false, glm::value_ptr(modelview2));
	//glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));
	//// glUniform1i(glGetUniformLocation(shader, "mode"), mode);
	glUniform3fv(glGetUniformLocation(shader, "diffuse"), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(shader, "ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(shader, "specular"), 1, glm::value_ptr(specular));
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(viewPos));
	glUniform1f(glGetUniformLocation(shader, "shininess"), shininess);


	// set light
	glUniform3fv(glGetUniformLocation(shader, "lightColor"), 1, glm::value_ptr(ext_light->getcolor()));
	glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, glm::value_ptr(ext_light->getpos()));
	glUniform1f(glGetUniformLocation(shader, "lightAtten"), ext_light->getatten());


	// Bind the VAO
	glBindVertexArray(VAO);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, vertexIndicesVec.size() * 3, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Geometry::draw(glm::mat4 C)
{

	//// Actiavte the shader program 
	glUseProgram(shader);

	//// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "cmatrix"), 1, false, glm::value_ptr(modelview));
	//glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(C));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));
	//// glUniform1i(glGetUniformLocation(shader, "mode"), mode);
	glUniform3fv(glGetUniformLocation(shader, "diffuse"), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(shader, "ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(shader, "specular"), 1, glm::value_ptr(specular));
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(eyePos));
	glUniform1f(glGetUniformLocation(shader, "shininess"), shininess);


	// set light
	glUniform3fv(glGetUniformLocation(shader, "lightColor"), 1, glm::value_ptr(light->getcolor()));
	glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, glm::value_ptr(light->getpos()));
	glUniform1f(glGetUniformLocation(shader, "lightAtten"), light->getatten());


	// Bind the VAO
	glBindVertexArray(VAO);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, vertexIndicesVec.size() * 3, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Geometry::set_material(glm::vec3 ambvec, glm::vec3 diffvec, glm::vec3 specvec, float shin, glm::vec3 col)
{
	ambient = ambvec;
	diffuse = diffvec;
	specular = specvec;
	shininess = shin;
	color = col;
}

void Geometry::update_eyePos(glm::vec3 eye)
{
	eyePos = eye;
}

void Geometry::scale(float factor)
{

	model = glm::scale(model, glm::vec3(factor, factor, factor));
}

void Geometry::translate(glm::vec3 loc)
{
	model = glm::translate(loc) * model;
}

void Geometry::rotate(glm::vec3 axis, float deg, int mode)
{
	// mode 1 is in place, mode 0 is around center
	if (mode == 1) {
		model = model * glm::rotate(glm::radians(deg), axis);
	}
	else {
		model = glm::rotate(glm::radians(deg), axis) * model;
	}

}

void Geometry::set_loc(glm::vec3 newloc)
{
	location = newloc;
}

void Geometry::set_orientation(glm::vec3 new_orientation)
{
	orientation = glm::normalize(new_orientation);
}

void Geometry::update_astro()
{

	glm::mat4 rot;

	glm::vec3 axis = glm::cross(glm::vec3(0.0, 0.0, 1.0), orientation);
	glm::vec3 da = orientation;
	glm::vec3 db = glm::vec3(0.0, 0.0, 1.0);
	float angle = glm::acos(glm::dot(db, da));
	if (orientation.z == -1.0 || orientation.z == 1.0) {
		axis = glm::vec3(0.0, 1.0, 0.0);
	}
	rot = glm::rotate(angle, axis);
	

	model = glm::translate(location) * rot * glm::mat4(1);
}

void Geometry::move(float speed)
{
	location = location + speed * orientation;
	update_astro();
}
