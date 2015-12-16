#include "ColliderTree.h"
#include <iostream>
ColliderTree::ColliderTree(vec3 c, vec3 e)
{
	center = c;
	halfwidth = e;

	root = new Node(c, e);
}

void ColliderTree::print()
{
	std::cout << "Collider Tree";
	if (root != nullptr) root->print();
}

bool ColliderTree::collidesWith(Cube* otherColliderPtr)
{
	return root->collidesWith(otherColliderPtr);
}

void ColliderTree::add(Cube* otherColliderPtr)
{
	root->addCollider(otherColliderPtr);
}

vector<Cube*> ColliderTree::collidedObjects(Cube* otherColliderPtr)
{
	return root->checkCollisions(otherColliderPtr);
}

ColliderTree::ColliderTree()
{
}


ColliderTree::~ColliderTree()
{
}
