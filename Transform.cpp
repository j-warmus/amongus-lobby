#include "Transform.h"

Transform::Transform(glm::mat4 trans_matrix)
{
	M = trans_matrix;
}

void Transform::update(glm::mat4 C)
{
	M = C * M;
}

void Transform::addChild(Node* new_child)
{
	children.push_back(new_child);
}

void Transform::draw(glm::mat4 C)
{
	glm::mat4 new_M;
	new_M = C * M;

	for (auto const& child : children) {
		child->draw(new_M);
	}
}

void Transform::update_eyePos(glm::vec3 eye)
{
	for (auto const& child : children) {
		child->update_eyePos(eye);
	}
}

void Transform::update_modelview(glm::mat4 M)
{
	for (auto const& child : children) {
		child->update_modelview(M);
	}
}
