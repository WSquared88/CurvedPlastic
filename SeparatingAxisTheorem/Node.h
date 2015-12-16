#pragma once
#include <vector>
#include <glm\gtx\transform.hpp>
#include "Cube.h"
class Node
{
public:

	Cube binBox;

	int count = 0;
	const int capacity = 4;

	std::vector<Cube*> colliderPtrs;

	bool isLeaf = true;

	Node* leftChild = nullptr;
	Node* rightChild = nullptr;
	Node* upChild = nullptr;
	Node* downChild = nullptr;

	Node(vec3 center, vec3 halfWidth);

	void print();

	bool collidesWith(Cube* colliderPtr);

	void addCollider(Cube* colliderPtr);

	vector<Cube*> checkCollisions(Cube* otherColliderPtr);

	void branch();
	
	~Node();
};

