#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

#include "ShaderHelpers.h"
#include "Shape.h"
#include "Triangle.h"
#include "Cube.h"
#include "GameWorld.h"
#include "ObjLoader.h"
#include "Camera.h"

using namespace std;
using namespace glm;

Shape* triangle;
Shape* cube;
Shape* carShape;
Shape* teapot;

float r;
float g;
float b;
GLint shaderIndex;
GLuint VAO;
GLuint VBO;
GLFWwindow* window;

double lastFrameTime;
extern bool mouseButtonHeld;
extern bool isWireFrame;
extern Camera* camera;
int currentIndex = 0;
float dt;

Triangle** ObjectList;
Cube* model;
Cube* plane;
Cube* car;
//Cube* teapotModel;

void init()
{
	glEnable(GL_DEPTH_BUFFER);
	glEnable(GL_DEPTH_TEST);

	cout << "initializing..." << endl;
	GLchar* vShaderCode;
	GLchar* fShaderCode;
	int length = 0;

	shaderIndex = loadShaderProgram("Shaders/vertexShader.glsl", "Shaders/fragmentShader.glsl");

	if (!shaderIndex)
	{
		cout << "Shader loading failed" << endl;
		return;
	}

	glUseProgram(shaderIndex);

	GLfloat vertexData[6 * 3];

#pragma region triangle
	//1
	vertexData[0] = -1;
	vertexData[1] = 1;
	vertexData[2] = -1;
	vertexData[3] = 1;
	vertexData[4] = 0;
	vertexData[5] = 0;
	//2
	vertexData[6] = -1;
	vertexData[7] = -1;
	vertexData[8] = -1;
	vertexData[9] = 0;
	vertexData[10] = 1;
	vertexData[11] = 0;
	//3
	vertexData[12] = 1;
	vertexData[13] = -1;
	vertexData[14] = -1;
	vertexData[15] = 0;
	vertexData[16] = 0;
	vertexData[17] = 1;
#pragma endregion triangle

	//triangle = new Shape(vertexData, 3, shaderIndex);
	//ObjectList = new Triangle*[100];

	//for (int i = 0; i < 100; i++)
	//{
	//	//ObjectList[i] = new Triangle(triangle, shaderIndex, vec3(0, 0, 0), 0, .08f, vec3(.1,.1,.1), vec3(.47f, .318f, .663f));
	//}

	vector<GLfloat> faces;
	vector<GLushort> numFaces;
	//vector<GLfloat> teapotFaces;
	//vector<GLushort> teapotNumFaces;
	vector<vec3> verts;
	//vector<vec3> teapotVerts;

	loadObj("box.obj", faces, numFaces, verts);
	//loadObj("teapot.obj", teapotFaces, teapotNumFaces, teapotVerts);
	cout << "done loading" << endl;

	cout << faces.size() << endl;
	GLfloat* modelVerts = &(faces[0]);
	cout << "convert verts" << endl;
	GLushort* modelFaces = &(numFaces[0]);
	cout << "convert faces" << endl;
	//GLfloat* teaVerts = &(teapotFaces[0]);
	//GLushort* teaFaces = &(teapotNumFaces[0]);

	cube = new Shape(modelVerts, numFaces.size(), shaderIndex, "archer.jpg");
	carShape = new Shape(modelVerts, numFaces.size(), shaderIndex, "archer.jpg");
	//teapot = new Shape(teaVerts, teapotNumFaces.size(), shaderIndex, "archer.jpg");

	model = new Cube(cube, shaderIndex, modelFaces, numFaces.size(), vec3(5, 1, 0), 0, .03f, vec3(.1, .1, .1), vec3(238, 130, 238), verts);
	car = new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(5, 1, 0), 180, 0.0f, vec3(.1, .05, .2), vec3(238, 130, 238), verts);
	plane = new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(-5, 0, -5), 0, 0.0f, vec3(10, 0.001, 10), vec3(238, 130, 238), verts);
	//teapotModel = new Cube(teapot, shaderIndex, teaFaces, teapotNumFaces.size(), vec3(0, 0, 0), 45, 0.0f, vec3(.1, .1, .1), vec3(238, 130, 238), teapotVerts);
	camera = new Camera();

	glClearColor(r, g, b, 1.0f);
}

inline bool CollisionCheck(Cube* A, Cube* B)
{

	vector<vec3> Axes = { A->OBB.axes[0], A->OBB.axes[1], A->OBB.axes[2], B->OBB.axes[0], B->OBB.axes[1], B->OBB.axes[2]  };
	vector<vec3> AAxes = { A->OBB.axes[0], A->OBB.axes[1], A->OBB.axes[2] };
	vector<vec3> BAxes = { B->OBB.axes[0], B->OBB.axes[1], B->OBB.axes[2] };


	vector<vec3> aCorners = vector<vec3>();
	vector<vec3> bCorners = vector<vec3>();

	for (int i = 0; i < AAxes.size(); i++)
	{
		for (int j = 0; j < BAxes.size(); j++)
		{
			Axes.push_back(cross(AAxes[i], BAxes[j]));
		}
	}

	
	for (int x = -1; x < 2; x += 2)
	{
		for (int y = -1; y < 2; y += 2)
		{
			for (int z = -1; z < 2; z += 2)
			{
				aCorners.push_back(A->OBB.c + ((float)x * A->OBB.axes[0] * A->OBB.halfWidths[0]) + ((float)y * A->OBB.axes[1] * A->OBB.halfWidths[1]) + ((float)z * A->OBB.axes[2] * A->OBB.halfWidths[2]));
			}
		}
	}

	for (int x = -1; x < 2; x += 2)
	{
		for (int y = -1; y < 2; y += 2)
		{
			for (int z = -1; z < 2; z += 2)
			{
				bCorners.push_back(B->OBB.c + ((float)x * B->OBB.axes[0] * B->OBB.halfWidths[0]) + ((float)y * B->OBB.axes[1] * B->OBB.halfWidths[1]) + ((float)z * B->OBB.axes[2] * B->OBB.halfWidths[2]));
			}
		}
	}

	for (int axis = 0; axis < Axes.size(); axis++)
	{
		vec3 rA = vec3(0, 0, 0);
		vec3 rB = vec3(0, 0, 0);

		vec3 currentAxis = Axes[axis];
		for (int i = 0; i < aCorners.size(); i++)
		{
			vec3 aVert = aCorners[i];
			if (dot(currentAxis, aVert - A->OBB.c) > dot(currentAxis, rA))
			{
				rA = aVert - A->OBB.c;
			}

		}

		for (int j = 0; j < bCorners.size(); j++)
		{
			vec3 bVert = bCorners[j];
			if (dot(currentAxis, bVert - B->OBB.c) > dot(currentAxis, rB))
			{
				rB = bVert - B->OBB.c;
			}
		}
		float left = abs(dot(currentAxis, A->OBB.c - B->OBB.c));
		float right = abs(dot(currentAxis, rA)) + abs(dot(currentAxis, rB));
		if (left <= right)
		{
			return false;
		}
	}


	return true;
}

void update()
{
	float currentTime = (float)glfwGetTime() * 1000;
	dt = currentTime - lastFrameTime;
	lastFrameTime = currentTime;

	//x = cos(pitch) * sin(yaw)
	//y = sin(pitch)
	//z = -cos(pitch) * cos(yaw)

	if (mouseButtonHeld)
	{
		//currentIndex = currentIndex % 100;
		//if (!ObjectList[currentIndex]->isVisible)
		//{
		//	//ObjectList[currentIndex]->isVisible = true;
		//}

		/*ObjectList[currentIndex]->currentPos = getCursorPos(window);
		ObjectList[currentIndex]->force = vec3(((rand() % 100) - 50) / 100000.0f, ((rand() % 100) - 50) / 100000.0f, 0);
		ObjectList[currentIndex]->rotNum = rand() % 100 / 100;
		currentIndex++;*/
	}

	/*for (int i = 0; i < 100; i++)
	{
	if (ObjectList[i]->isVisible)
	{
	ObjectList[i]->Update(dt);
	}
	}*/
	car->Update(dt);
	model->Update(dt);
	//teapotModel->Update(dt);
	camera->position = car->currentPos + vec3(2 * sin(car->rotNum), 1, 2 * cos(car->rotNum));
	mat4 ProjectionMatrix = camera->getProjectionMatrix();
	//mat4 ViewMatrix = camera->getViewMatrix();
	mat4 ViewMatrix = lookAt(camera->position, car->currentPos, vec3(0, 1, 0));
	mat4 ModelMatrix = mat4(1.0);
	mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	
	setShaderMatrix(shaderIndex, "cameraMatrix", MVP);

	if (CollisionCheck(model, car))
	{
		cout << "Collision" << endl;
	}

	if (isWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*for (int i = 0; i < 100; i++)
	{
	if (ObjectList[i]->isVisible)
	{
	ObjectList[i]->Draw();
	}
	}*/

	model->Draw();
	//teapotModel->Draw();
	car->Draw();
	plane->Draw();
	glFlush();
}

int main(int argc, char** argv)
{
	if (!glfwInit())
	{
		return -1;
	}

	window = glfwCreateWindow(800, 800, "Curved Plastic", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetMouseButtonCallback(window, mouseClick);
	glfwSetCursorPosCallback(window, mouseMove);

	glfwSetKeyCallback(window, keyPress);

	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	r = 0.392f;
	g = 0.584f;
	b = 0.929f;

	glClearColor(r, g, b, 1.0f);

	init();

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();

	return 1;
}