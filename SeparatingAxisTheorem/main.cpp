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
bool bulletFired;

Triangle** ObjectList;
Cube* model;
Cube* plane;
Cube* car;
Cube* bullet;
vector<Cube*> obstacles;
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

	model = new Cube(cube, shaderIndex, modelFaces, numFaces.size(), vec3(5, 0, 0), 0, .03f, vec3(.1, .1, .1), vec3(238, 130, 238), verts);
	car = new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(0, 0.1, 0), 180, 0.0f, vec3(.1, .05, .2), vec3(238, 130, 238), verts);
	plane = new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(-5, 0, -5), 0, 0.0f, vec3(10, 0.001, 10), vec3(238, 130, 238), verts);
	bullet = new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(-10000, -10000, -10000), 0, 0.0f, vec3(0.05, 0.05, 0.05), vec3(238, 130, 238), verts);
	obstacles = vector<Cube*>();
	// keep any obstacles added within these X (-14 , 4) and Z (-4 , 14) values
	obstacles.push_back(new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(4, 0.1, 0), 180, 0.0f, vec3(.1, .1, .1), vec3(0, 0, 0), verts));
	obstacles.push_back(new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(0, 0.1, 14), 180, 0.0f, vec3(.1, .1, .1), vec3(0, 0, 0), verts));
	obstacles.push_back(new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(-14, 0.1, 0), 180, 0.0f, vec3(.1, .1, .1), vec3(0, 0, 0), verts));
	obstacles.push_back(new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(0, 0.1, -4), 180, 0.0f, vec3(.1, .1, .1), vec3(0, 0, 0), verts));
	obstacles.push_back(new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(2, 0.1, 12), 180, 0.0f, vec3(.1, .1, .1), vec3(0, 0, 0), verts));
	obstacles.push_back(new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(3, 0.1, 10), 180, 0.0f, vec3(.1, .1, .1), vec3(0, 0, 0), verts));
	obstacles.push_back(new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(-12, 0.1, 0), 180, 0.0f, vec3(.1, .1, .1), vec3(0, 0, 0), verts));
	obstacles.push_back(new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(-10, 0.1, -1), 180, 0.0f, vec3(.1, .1, .1), vec3(0, 0, 0), verts));
	obstacles.push_back(new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(1, 0.1, 5), 180, 0.0f, vec3(.1, .1, .1), vec3(0, 0, 0), verts));
	obstacles.push_back(new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(-8, 0.1, 7), 180, 0.0f, vec3(.1, .1, .1), vec3(0, 0, 0), verts));
	obstacles.push_back(new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(-9, 0.1, 3), 180, 0.0f, vec3(.1, .1, .1), vec3(0, 0, 0), verts));
	obstacles.push_back(new Cube(carShape, shaderIndex, modelFaces, numFaces.size(), vec3(0, 0.1, -4), 180, 0.0f, vec3(.1, .1, .1), vec3(0, 0, 0), verts));
	//teapotModel = new Cube(teapot, shaderIndex, teaFaces, teapotNumFaces.size(), vec3(0, 0, 0), 45, 0.0f, vec3(.1, .1, .1), vec3(238, 130, 238), teapotVerts);
	camera = new Camera();

	glClearColor(r, g, b, 1.0f);
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
	bullet->Update(dt);
	// check the bullet's velocity, stop drawing if really small
	vec3 velTest = bullet->velocity;
	if (velTest.x < 0)
	{
		velTest.x *= -1;
	}
	if (velTest.y < 0)
	{
		velTest.y *= -1;
	}
	if (velTest.z < 0)
	{
		velTest.z *= -1;
	}
	if (velTest.x < 0.0001 && velTest.y < 0.0001 && velTest.z < 0.0001)
	{
		bulletFired = false;
		bullet->isVisible = false;
		bullet->currentPos = vec3(-10000, -10000, -10000);
	}
	for (int i = 0; i < obstacles.size(); i++)
	{
		obstacles[i]->Update(dt);
	}
	//model->Update(dt);
	//teapotModel->Update(dt);
	camera->position = car->currentPos + vec3(2 * sin(car->rotNum), 1, 2 * cos(car->rotNum));
	mat4 ProjectionMatrix = camera->getProjectionMatrix();
	//mat4 ViewMatrix = camera->getViewMatrix();
	mat4 ViewMatrix = lookAt(camera->position, car->currentPos, vec3(0, 1, 0));
	mat4 ModelMatrix = mat4(1.0);
	mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	
	setShaderMatrix(shaderIndex, "cameraMatrix", MVP);

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

	//model->Draw();
	//teapotModel->Draw();
	car->Draw();
	// only draw visible bullet
	if (bullet->isVisible == true)
	{
		bullet->Draw();
	}
	for (int i = 0; i < obstacles.size(); i++)
	{
		obstacles[i]->Draw();
	}
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