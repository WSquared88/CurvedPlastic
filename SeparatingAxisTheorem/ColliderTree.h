#pragma once
#include "Node.h"
class ColliderTree
{
public:
	vec3 center, halfwidth;

	Node* root = nullptr;

	ColliderTree(vec3 c, vec3 e);

	void print();
	bool collidesWith(Cube* otherColliderPtr);
	void add(Cube* otherColliderPtr);

	vector<Cube*> collidedObjects(Cube* otherColliderPtr);

	ColliderTree();
	~ColliderTree();
};

