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
		default:				break;
		}
	}

	GLuint* getIndicesArray(ObjectType type, unsigned int& size) {
		switch (type) {
		case grass: return grassIndices(size);
		default:				break;
		}
	}

private:
	GLfloat* grassVertices(unsigned int& size) {
		static GLfloat vertices[] = {
			// coordinates			// color				// texture			//normals
			 6.0f, 0.0f,  6.0f,		1.0f, 0.0f, 0.0f,		0.0f,  2.0f,	0.0f, 1.0f, 0.0f,
			-6.0f, 0.0f,  6.0f,		0.0f, 1.0f, 0.0f,		0.0f,  0.0f,	0.0f, 1.0f, 0.0f,
			-6.0f, 0.0f, -6.0f,		0.0f, 0.0f, 1.0f,		2.0f,  0.0f,	0.0f, 1.0f, 0.0f,
			 6.0f, 0.0f, -6.0f,		1.0f, 0.0f, 1.0f,		2.0f,  2.0f,	0.0f, 1.0f, 0.0f
		};
		cout << "kutas";
		size = sizeof(vertices);
		return vertices;
	}

	GLuint* grassIndices(unsigned int &size) {
		static GLuint indices[] = {
			0, 1, 2,
			0, 2, 3
		};
		cout << "pe";
		size = sizeof(indices);
		return indices;
	}
};