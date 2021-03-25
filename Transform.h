#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Node.h"
#include <list>

class Transform : public Node
{
public:
	glm::mat4 M;
	std::list<Node*> children;
	Transform(glm::mat4 trans_matrix);
	void update(glm::mat4 C);
	void addChild(Node* new_child);
	void draw(glm::mat4 C);
	void update_eyePos(glm::vec3 eye);
	void update_modelview(glm::mat4 M);
};

#endif

