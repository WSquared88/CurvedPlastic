#pragma once

#include <vector>
#include <string>
#include <glfw3.h>
#include <glm\glm.hpp>

using namespace std;
using namespace glm;

bool loadObj(const char* path, vector<GLfloat> &fullVerts, vector<GLushort> &out_faces, vector<vec3> &out_verts);

void fillFaces(vector<GLfloat> &fullVerts, vector<string> &subString, int i, vector<vec3> &temp_vertices, vector<vec2> &temp_uvs, vector<vec3> &temp_normals);

vector<string> splitString(string fullString, string const delimeter);