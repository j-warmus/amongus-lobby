#include "Model.h"
#include "sstream"
#include "fstream"
#include "iostream"

Model::Model(std::string objFilename, GLfloat pointSize) 
	: pointSize(pointSize)
{
	GLfloat xmax, xmin, ymax, ymin, zmax, zmin, xmid, ymid, zmid;
	xmax = xmin = ymax = ymin = zmax = zmin = xmid = ymid = zmid = 0;
	GLfloat scalefactor = 7.5, downfactor = 1;
	// mode = 0;


	/* 
	 * TODO: Section 2: Currently, all the points are hard coded below. 
	 * Modify this to read points from an obj file.
	 */
	
	std::ifstream objFile(objFilename); // The obj file we are reading.

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

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the 
				// coordinates. 
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				//Find max and min of each.  This code is really bad but I don't remember enough C++ right now to do better
				if (point.x > xmax){
					xmax = point.x;
				}
				else if (point.x < xmin) {
					xmin = point.x;
				}
				if (point.y > ymax) {
					ymax = point.y;
				}
				else if (point.y < ymin) {
					ymin = point.y;
				}
				if (point.z > zmax) {
					zmax = point.z;
				}
				else if (point.z < zmin) {
					zmin = point.z;
				}
				

				// Process the point. For example, you can save it to a.
				points.push_back(point);
			//load vertex normals
			} else if (label == "vn") {
				glm::vec3 normal;
				ss >> normal.x >> normal.y >> normal.z;
				glm::normalize(normal);
				normals.push_back(normal);

			//load point and no
			} else if (label == "f") {
				glm::ivec3 pointIndex, normalIndex;
				std::string s_x, s_y, s_z;
				int a;
				ss >> s_x >> s_y >> s_z;
				a = sscanf(s_x.c_str(), "%u//%u", &pointIndex.x, &normalIndex.x);
				a = sscanf(s_y.c_str(), "%u//%u", &pointIndex.y, &normalIndex.y);
				a = sscanf(s_z.c_str(), "%u//%u", &pointIndex.z, &normalIndex.z);
				pointIndex.x -= 1;
				normalIndex.x -= 1;
				pointIndex.y -= 1;
				normalIndex.y -= 1;
				pointIndex.z -= 1;
				normalIndex.z -= 1;
				pointIndices.push_back(pointIndex);
				normalIndices.push_back(normalIndex);
			}
		}
	}
	else
	{
		std::cerr << "Can't open the file " << objFilename << std::endl;
	}
	objFile.close();


	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen. 
	 */
	xmid = (xmax + xmin) / 2;
	ymid = (ymax + ymin) / 2;
	zmid = (zmax + zmin) / 2;
	
	//scale to fit in 1 x 1 x 1 box
	if ((xmax - xmid > ymax - ymid) && (xmax - xmid > zmax - zmid)) {
		downfactor = xmax - xmid;
	}
	else if ((ymax - ymid > xmax - xmid) && (ymax - ymid > zmax - zmid)) {
		downfactor = ymax - ymid;
	}
	else {
		downfactor = zmax - zmid;
	}
	center = glm::vec3(xmid / downfactor, ymid / downfactor, zmid / downfactor);
	// center, scale down, then scale up.

	for (auto& item: points) {
		item.x = item.x - xmid;
		item.x = (item.x / downfactor) * scalefactor;
		item.y = item.y - ymid;
		item.y = (item.y / downfactor) * scalefactor;
		item.z = item.z - zmid;
		item.z = (item.z / downfactor) * scalefactor;
	}

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);


	// Generate a vertex array (VAO) and vertex buffer object (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &points_VBO); 
	glGenBuffers(1, &normals_VBO);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* points.size(), points.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass the vertex data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind VBO to the bound VAO, and store the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normals.size(), normals.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass the vertex data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3)* pointIndices.size(), pointIndices.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

Model::~Model() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &points_VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
}

void Model::draw(const glm::mat4& view, const glm::mat4& projection, glm::vec3 viewPos, Light* light)
{

	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));
	// glUniform1i(glGetUniformLocation(shader, "mode"), mode);
	glUniform3fv(glGetUniformLocation(shader, "diffuse"), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(shader, "ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(shader, "specular"), 1, glm::value_ptr(specular));
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(viewPos));
	glUniform1f(glGetUniformLocation(shader, "shininess"), shininess);


	// set light
	glUniform3fv(glGetUniformLocation(shader, "lightColor"), 1, glm::value_ptr(light->getcolor()));
	glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, glm::value_ptr(light->getpos()));
	glUniform1f(glGetUniformLocation(shader, "lightAtten"), light->getatten());


	// Bind the VAO
	glBindVertexArray(VAO);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, pointIndices.size()*3, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Model::update()
{
	// Spin the cube by 1 degree
	// spin(0.1f);
}

void Model::updatePointSize(GLfloat size)
{
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 * GLfloat size
	 */
	pointSize = size;
}

void Model::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Model::scale(float factor)
{

	model = glm::scale(model,glm::vec3(factor,factor,factor));
}

void Model::translate(glm::vec3 loc) 
{
	model = glm::translate(loc) * model;
}

void Model::rotate(glm::vec3 axis, float deg, int mode)
{
	// mode 1 is in place, mode 0 is around center
	if (mode == 1) {
		model = model * glm::rotate(glm::radians(deg), axis);
	}
	else {
		model = glm::rotate(glm::radians(deg), axis) * model;
	}
	
}

//void Model::swap_mode()
//{
//	if (mode == 0) {
//		mode = 1;
//	}
//	else {
//		mode = 0;
//	}
//}

void Model::set_material(glm::vec3 ambvec, glm::vec3 diffvec, glm::vec3 specvec, float shin, glm::vec3 col)
{
	ambient = ambvec;
	diffuse = diffvec;
	specular = specvec;
	shininess = shin;
	color = col;
}
glm::vec3 Model::return_center() {
	return center;
}