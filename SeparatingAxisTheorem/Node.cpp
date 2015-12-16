#include "Node.h"
#include <iostream>
#include <iomanip>


Node::Node(vec3 center, vec3 halfWidth)
{
	binBox = Cube();
	binBox.OBB.c = center;
	binBox.OBB.halfWidths = halfWidth;
	binBox.worldMatrix = translate(center) * rotate(0.0f, vec3(0, 1, 0)) * scale(vec3(1, 1, 1));
	binBox.OBB.axes = vector<vec3> { vec3(1, 0, 0), vec3(0, 1, 0), vec3(0, 0, 1) };
	colliderPtrs = std::vector<Cube*>(capacity);
	for each (Cube* colliderPtr in colliderPtrs) colliderPtr = nullptr;
}

void Node::print()
{
	std::cout << std::fixed << std::setprecision(2) << "Node: " << std::endl;
	std::cout << "binBox: ";
	binBox.print();
	std::cout << std::endl;

	std::cout << "Colliders: " << count << std::endl;
	for each (Cube* colliderPtr in colliderPtrs) if (colliderPtr != nullptr)
	{
		colliderPtr->print();
	}
	std::cout << std::endl;

	if (leftChild == nullptr) std::cout << "left child = nullptr" << std::endl;
	else
	{
		std::cout << "left child: ";
		leftChild->print();
	}
	if (rightChild == nullptr) std::cout << "right child = nullptr" << std::endl;
	else
	{
		std::cout << "right child: ";
		rightChild->print();
	}
	if (upChild == nullptr) std::cout << "up child = nullptr" << std::endl;
	else
	{
		std::cout << "up child: ";
		upChild->print();
	}
	if (downChild == nullptr) std::cout << "down child = nullptr" << std::endl;
	else
	{
		std::cout << "down child: ";
		downChild->print();
	}

}

bool Node::collidesWith(Cube* otherColliderPtr)
{
	//easy check: is the collider even colliding with the binBox
	if (!binBox.TestOBBOBB(*otherColliderPtr)) return false;

	//After that, if we are a leaf, check each collider in the binbox to see if they collide there
	if (isLeaf)
	{
		for each (Cube* colliderPtr in colliderPtrs)
		{
			if (colliderPtr != nullptr)
			{
				if (colliderPtr->TestOBBOBB(*otherColliderPtr))
					return true;
			}
		}
	}

	//If we aren't a leaf, check if our children collide
	if (!isLeaf) 
	{
		if (leftChild->collidesWith(otherColliderPtr)) return true;
		if (rightChild->collidesWith(otherColliderPtr)) return true;
		if (upChild->collidesWith(otherColliderPtr)) return true;
		if (downChild->collidesWith(otherColliderPtr)) return true;
	}

	return false;
}

void Node::addCollider(Cube* otherColliderPtr)
{
	//won't add if they don't collide with the binBox
	if (!binBox.TestOBBOBB(*otherColliderPtr))
		return;

	//if this node is at capacity, it will branch
	if (count + 1 > capacity)
		branch();
		//if this is a leaf, then add the collider to this node and increment count
	if (isLeaf)
		colliderPtrs[count++] = otherColliderPtr;
	else if (!isLeaf)
	{
		leftChild->addCollider(otherColliderPtr);
		rightChild->addCollider(otherColliderPtr);
		upChild->addCollider(otherColliderPtr);
		downChild->addCollider(otherColliderPtr);
	}
}

vector<Cube*> Node::checkCollisions(Cube* otherColliderPtr)
{
	if (!binBox.TestOBBOBB(*otherColliderPtr))
		return vector<Cube*>();

	if (!isLeaf)
	{
		vector<Cube*> collidedObjects = vector<Cube*>();

		vector<Cube*> leftCollisions = leftChild->checkCollisions(otherColliderPtr);
		vector<Cube*> rightCollisions = rightChild->checkCollisions(otherColliderPtr);
		vector<Cube*> upCollisions = upChild->checkCollisions(otherColliderPtr);
		vector<Cube*> downCollisions = downChild->checkCollisions(otherColliderPtr);

		for (int i = 0; i < leftCollisions.size(); i++)
		{
			Cube* cube = leftCollisions[i];
			collidedObjects.push_back(cube);
		}

		for (int i = 0; i < rightCollisions.size(); i++)
		{
			Cube* cube = rightCollisions[i];
			collidedObjects.push_back(cube);
		}


		for (int i = 0; i < upCollisions.size(); i++)
		{
			Cube* cube = upCollisions[i];
			collidedObjects.push_back(cube);
		}


		for (int i = 0; i < downCollisions.size(); i++)
		{
			Cube* cube = downCollisions[i];
			collidedObjects.push_back(cube);
		}

		return collidedObjects;
	}
	else
	{
		vector<Cube*> collidedObjects = vector<Cube*>();
		for (int i = 0; i < colliderPtrs.size(); i++)
		{
			Cube* cube = colliderPtrs[i];
			if (cube != nullptr)
			{
				if (otherColliderPtr->TestOBBOBB(*cube))
				{
					collidedObjects.push_back(cube);
				}
			}
		}

		return collidedObjects;
	}
}

void Node::branch()
{
	//We aren't a leaf now
	isLeaf = false;

	//make child nodes
	vec3 qWidth = vec3(binBox.OBB.halfWidths.x * .5, binBox.OBB.halfWidths.y, binBox.OBB.halfWidths.z * .5);

	leftChild = new Node(vec3(binBox.OBB.c.x - qWidth.x, binBox.OBB.c.y, binBox.OBB.c.z + qWidth.z), qWidth);
	rightChild = new Node(vec3(binBox.OBB.c.x + qWidth.x, binBox.OBB.c.y, binBox.OBB.c.z + qWidth.z), qWidth);
	upChild = new Node(vec3(binBox.OBB.c.x - qWidth.x, binBox.OBB.c.y, binBox.OBB.c.z - qWidth.z), qWidth);
	downChild = new Node(vec3(binBox.OBB.c.x + qWidth.x, binBox.OBB.c.y, binBox.OBB.c.z - qWidth.z), qWidth);
	//add this node's colliders to the the children
	//set this node's colliders to nullptr
	for (int i = 0; i < count; i++)
	{
		leftChild->addCollider(colliderPtrs[i]);
		rightChild->addCollider(colliderPtrs[i]);
		upChild->addCollider(colliderPtrs[i]);
		downChild->addCollider(colliderPtrs[i]);
		colliderPtrs[i] = nullptr;
	}

	count = 0;
}


Node::~Node()
{
	if (leftChild != nullptr) delete leftChild;
	if (rightChild != nullptr) delete rightChild;
	if (upChild != nullptr) delete upChild;
	if (downChild != nullptr) delete downChild;
}
