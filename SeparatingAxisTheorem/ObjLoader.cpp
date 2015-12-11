#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm\glm.hpp>

#include "ObjLoader.h"

using namespace std;
using namespace glm;

bool loadObj(const char* path, vector<GLfloat> &fullVerts, vector<GLushort> &out_faces, vector<vec3> &out_verts)
{
	cout << "loading object..." << endl;

	vector<vec2> temp_uvs;
	vector<vec3> temp_normals;
	int numFaces = 0;

	ifstream objFile(path, ios::in);

	if (!objFile.is_open())
	{
		cout << "Unable to open model file!" << endl;
		return false;
	}

	string line;

	while (getline(objFile, line))
	{
		vector<string> subString;

		subString = splitString(line, " ");

		if (subString.size() > 0)
		{
			if (subString[0] == "v")
			{
				vec3 tempVert;
				float xVert = stof(subString[1]);
				float yVert = stof(subString[2]);
				float zVert = stof(subString[3]);

				tempVert = vec3(xVert, yVert, zVert);
				out_verts.push_back(tempVert);
			}
			else if (subString[0] == "vt")
			{
				vec2 tempUV;
				float xUV = stof(subString[1]);
				float yUV = stof(subString[2]);

				tempUV = vec2(xUV, yUV);
				temp_uvs.push_back(tempUV);
			}
			else if (subString[0] == "vn")
			{
				vec3 tempNormal;
				float xNorm = stof(subString[1]);
				float yNorm = stof(subString[2]);
				float zNorm = stof(subString[3]);

				tempNormal = vec3(xNorm, yNorm, zNorm);
				temp_normals.push_back(tempNormal);
			}
			else if (subString[0] == "f")
			{
				if (subString.size()-1 == 3)
				{
					for (int i = 1; i < subString.size(); i++)
					{
						fillFaces(fullVerts, subString, i, out_verts, temp_uvs, temp_normals);
					}
					numFaces += subString.size() - 1;
				}
				else if (subString.size() - 1 == 4)
				{
					fillFaces(fullVerts, subString, 1, out_verts, temp_uvs, temp_normals);
					fillFaces(fullVerts, subString, 2, out_verts, temp_uvs, temp_normals);
					fillFaces(fullVerts, subString, 3, out_verts, temp_uvs, temp_normals);
					fillFaces(fullVerts, subString, 3, out_verts, temp_uvs, temp_normals);
					fillFaces(fullVerts, subString, 4, out_verts, temp_uvs, temp_normals);
					fillFaces(fullVerts, subString, 1, out_verts, temp_uvs, temp_normals);
					numFaces += 6;
				}
			}
		}
	}

	for (int i = 0; i < numFaces; i++)
	{
		out_faces.push_back((GLushort)i);
	}
	cout << "num faces " << numFaces << endl;
}

void fillFaces(vector<GLfloat> &fullVerts, vector<string> &subString, int i, vector<vec3> &temp_vertices, vector<vec2> &temp_uvs, vector<vec3> &temp_normals)
{
	vector<string> faceNumbers = splitString(subString[i], "/");
	int j = 0;

	if (temp_vertices.size() > 0)
	{
		int firstVert = stoi(faceNumbers[j]) - 1;
		j++;

		fullVerts.push_back(temp_vertices[firstVert].x);
		fullVerts.push_back(temp_vertices[firstVert].y);
		fullVerts.push_back(temp_vertices[firstVert].z);
	}
	else
	{
		fullVerts.push_back(0);
		fullVerts.push_back(0);
		fullVerts.push_back(0);
	}

	if (temp_uvs.size() > 0)
	{
		int firstUV = stoi(faceNumbers[j]) - 1;
		j++;

		fullVerts.push_back(temp_uvs[firstUV].x);
		fullVerts.push_back(temp_uvs[firstUV].y);
	}
	else
	{
		fullVerts.push_back(0);
		fullVerts.push_back(0);
	}

	if (temp_normals.size() > 0)
	{
		int firstNorm = stoi(faceNumbers[j]) - 1;
		j++;

		fullVerts.push_back(temp_normals[firstNorm].x);
		fullVerts.push_back(temp_normals[firstNorm].y);
		fullVerts.push_back(temp_normals[firstNorm].z);
	}
	else
	{
		fullVerts.push_back(0);
		fullVerts.push_back(0);
		fullVerts.push_back(0);
	}
}

vector<string> splitString(string fullString, string const delimeter)
{
	string splitString;
	int i = 0;
	int length = delimeter.length();
	vector<string> returnStrings;

	while (i < fullString.length())
	{
		splitString = "";
		while (i < fullString.length() && fullString.substr(i, length) != delimeter)
		{
			splitString.append(fullString.substr(i, length));
			i += length;
		}

		while (i < fullString.length() && fullString.substr(i, length) == delimeter)
		{
			i += length;
		}

		returnStrings.push_back(splitString);
	}

	return returnStrings;
}

vector<string> split(const string& to_split, char delim) 
{
	vector<string> result;
	result.push_back("");
	for (char c : to_split)
	{
		if (c == delim) 
		{
			result.push_back("");
		}
		else 
		{
			result.back().push_back(c);
		}
	}
	return result;
}