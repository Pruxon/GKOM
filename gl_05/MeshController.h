#pragma once
#include <GL/glew.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "EnumObjectType.h"

using namespace std;

class MeshController {
public:
	GLfloat* getVerticesArray(ObjectType type, unsigned int &size) {
		switch (type) {
		case grass: return grassVertices(size);
		case windmill: return windmillVertices(size);
		default:				break;
		}
	}

	GLuint* getIndicesArray(ObjectType type, unsigned int& size) {
		switch (type) {
		case grass: return grassIndices(size);
		case windmill: return windmillIndices(size);
		default:				break;
		}
	}

private:
	GLfloat* grassVertices(unsigned int& size) {
		static GLfloat vertices[] = {
			// coordinates			// color				// texture			//normals
			 60.0f, 0.0f,  60.0f,		1.0f, 0.0f, 0.0f,		0.0f,  20.0f,	0.0f, 1.0f, 0.0f,
			-60.0f, 0.0f,  60.0f,		0.0f, 1.0f, 0.0f,		0.0f,  0.0f,	0.0f, 1.0f, 0.0f,
			-60.0f, 0.0f, -60.0f,		0.0f, 0.0f, 1.0f,		20.0f,  0.0f,	0.0f, 1.0f, 0.0f,
			 60.0f, 0.0f, -60.0f,		1.0f, 0.0f, 1.0f,		20.0f,  20.0f,	0.0f, 1.0f, 0.0f
		};
		
		size = sizeof(vertices);
		return vertices;
	}

	GLuint* grassIndices(unsigned int &size) {
		static GLuint indices[] = {
			0, 1, 2,
			0, 2, 3
		};
		size = sizeof(indices);
		return indices;
	}

	GLfloat* windmillVertices(unsigned int& size) {
		static GLfloat vertices[] =
		{
			// coordinates				// color				// texture				//normals
			0.20f,  1.3f,   0.005f,		1.0f, 0.0f, 0.0f,		1.0f,  0.0f,	1.0f, 1.0f, 1.0f,
		   -0.20f,  1.3f,   0.005f,	    0.0f, 1.0f, 0.0f,		0.0f,  0.0f,	-1.0f, 1.0f, 1.0f,
		   -0.12f,  0.13f,   0.005f,	    0.0f, 0.0f, 1.0f,		0.0f,  2.0f,	-1.0f, -1.0f, 1.0f,
			0.12f,  0.13f,   0.005f,		1.0f, 0.0f, 1.0f,		1.0f,  2.0f,	1.0f, -1.0f, 1.0f,

			0.20f,  1.3f,  -0.005f,		1.0f, 0.0f, 0.0f,		0.0f,  0.0f,	1.0f, 1.0f, -1.0f,
			-0.20f,  1.3f,  -0.005f,		0.0f, 1.0f, 0.0f,		1.0f,  0.0f,	-1.0f, 1.0f, -1.0f,
		   -0.12f,  0.13f,  -0.005f,		0.0f, 0.0f, 1.0f,		1.0f,  2.0f,	-1.0f, -1.0f, -1.0f,
			0.12f,  0.13f,  -0.005f,		1.0f, 0.0f, 1.0f,		0.0f,  2.0f,	1.0f, -1.0f, -1.0f,

			//duplication
			0.20f,  1.3f,   0.005f,		1.0f, 0.0f, 0.0f,		1.0f,  0.0f,	1.0f, 1.0f, 1.0f,
			-0.20f,  1.3f,   0.005f,	    0.0f, 1.0f, 0.0f,		0.9f,  0.0f,	-1.0f, 1.0f, 1.0f,
		   -0.12f,  0.13f,   0.005f,	    0.0f, 0.0f, 1.0f,		0.9f,  2.0f,	-1.0f, -1.0f, 1.0f,
			0.12f,  0.13f,   0.005f,		1.0f, 0.0f, 1.0f,		1.0f,  2.0f,	1.0f, -1.0f, 1.0f,

			0.20f, 1.3f,  -0.005f,		1.0f, 0.0f, 0.0f,		0.9f,  0.0f,	1.0f, 1.0f, -1.0f,
			-0.20f,  1.3f,  -0.005f,		0.0f, 1.0f, 0.0f,		1.0f,  0.0f,	-1.0f, 1.0f, -1.0f,
		   -0.12f,  0.13f,  -0.005f,		0.0f, 0.0f, 1.0f,		1.0f,  1.0f,	-1.0f, -1.0f, -1.0f,
			0.12f,  0.13f,  -0.005f,		1.0f, 0.0f, 1.0f,		0.9f,  1.0f,	1.0f, -1.0f, -1.0f
		};

		size = sizeof(vertices);
		return vertices;
	}

	GLuint* windmillIndices(unsigned int& size) {
		static GLuint indices[] = {
			0, 1, 2,
			0, 2, 3,

			8, 11, 15,
			8, 15, 12,

			4, 5, 6,
			4, 7, 6,

			9, 13, 14,
			9, 10, 14
		};
		size = sizeof(indices);
		return indices;
	}
};