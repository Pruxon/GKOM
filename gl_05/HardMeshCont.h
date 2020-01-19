#pragma once


#include <GL/glew.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "EnumObjectType.h"

using namespace std;

class HardMeshCont {
public:
	HardMeshCont() {}

	GLfloat* getVerticesArray(HardTypeOfObject type, int numberOfFaces, GLfloat radius, GLfloat height, unsigned int& size) {
		switch (type)
		{
		case cylinder:		return cylinderVertices(numberOfFaces, radius, height, size);
		default:			break;
		}
	}

	GLuint* getIndicesArray(HardTypeOfObject type, int numberOfFaces, unsigned int& size) {
		switch (type)
		{
		case cylinder:			return cylinderIndices(numberOfFaces, size);
		default:				break;
		}
	}

private:
	GLfloat* cylinderVertices(int numberOfFaces, GLfloat radius, GLfloat height, unsigned int& size) {
		static GLfloat* vertices = new GLfloat[(numberOfFaces * 4 + 2) * 11];

		for (int i = 0; i < (numberOfFaces * 4 + 2) * 11; i++) {
			vertices[i] = 0.0f;
		}

		//points 0 and 1
		vertices[1] = height;
		//normal
		vertices[9] = 1.0f;

		vertices[(numberOfFaces + 2) * 11] = radius;
		vertices[(numberOfFaces + 2) * 11 + 1] = height;
		//normal
		vertices[(numberOfFaces + 2) * 11 + 9] = 1.0f;


		//texture
		vertices[6] = 0.75f;
		vertices[7] = 0.75f;

		vertices[17] = 0.75f;
		vertices[18] = 0.75f;

		//normal
		vertices[20] = -1.0f;

		//point nr 2
		vertices[22] = radius;
		vertices[28] = 1.0f;
		vertices[29] = 0.75f;

		//normal
		vertices[31] = -1.0f;

		//point obove point nr 2
		vertices[28 + numberOfFaces * 11] = 1.0f;
		vertices[29 + numberOfFaces * 11] = 0.75f;
		//normal
		vertices[31 + numberOfFaces * 11] = 1.0f; //niepotrzebne

		//obracanie - macierz
		glm::mat4 rotationZ; //macierz rotacji
		rotationZ = glm::rotate(rotationZ, glm::radians(360.0f / numberOfFaces), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec4 currentV = glm::vec4(radius, 0.0f, 0.0f, 1.0f);//wekor z wierzcholkiem 3d
		glm::vec4 currentTexturePos = glm::vec4(0.25f, 0.0f, 0.0f, 1.0f);//wektor z pozycja wierzcholka na teksturze
		for (int i = 0; i < numberOfFaces - 1; i++) {
			currentV = rotationZ * currentV;
			currentTexturePos = rotationZ * currentTexturePos;

			vertices[33 + i * 11] = currentV[0];
			vertices[35 + i * 11] = currentV[2];
			//normals
			vertices[42 + i * 11] = -1.0f;

			vertices[33 + numberOfFaces * 11 + i * 11] = currentV[0];
			vertices[34 + numberOfFaces * 11 + i * 11] = height;
			vertices[35 + numberOfFaces * 11 + i * 11] = currentV[2];
			//normals
			vertices[42 + numberOfFaces * 11 + i * 11] = 1.0f;

			vertices[39 + i * 11] = currentTexturePos[0] + 0.75f;
			vertices[40 + i * 11] = currentTexturePos[2] + 0.75f;

			vertices[39 + numberOfFaces * 11 + i * 11] = currentTexturePos[0] + 0.75f;
			vertices[40 + numberOfFaces * 11 + i * 11] = currentTexturePos[2] + 0.75f;
		}

		//vertices duplication for proper texturing

		for (int i = 0; i < numberOfFaces; i++) {
			vertices[22 + numberOfFaces * 2 * 11 + i * 11] = vertices[22 + i * 11];
			vertices[24 + numberOfFaces * 2 * 11 + i * 11] = vertices[24 + i * 11];
			//normals
			vertices[30 + numberOfFaces * 2 * 11 + i * 11] = vertices[22 + i * 11];
			vertices[32 + numberOfFaces * 2 * 11 + i * 11] = vertices[24 + i * 11];



			vertices[22 + numberOfFaces * 3 * 11 + i * 11] = vertices[22 + numberOfFaces * 11 + i * 11];
			vertices[23 + numberOfFaces * 3 * 11 + i * 11] = vertices[23 + numberOfFaces * 11 + i * 11];
			vertices[24 + numberOfFaces * 3 * 11 + i * 11] = vertices[24 + numberOfFaces * 11 + i * 11];
			//normals
			vertices[30 + numberOfFaces * 3 * 11 + i * 11] = vertices[22 + numberOfFaces * 11 + i * 11];
			vertices[32 + numberOfFaces * 3 * 11 + i * 11] = vertices[24 + numberOfFaces * 11 + i * 11];

			//texture
			if (i % 2 == 0) {
				vertices[28 + numberOfFaces * 2 * 11 + i * 11] = 0.5f / numberOfFaces * 4;
				vertices[28 + numberOfFaces * 3 * 11 + i * 11] = 0.5f / numberOfFaces * 4;
			}

			vertices[29 + numberOfFaces * 3 * 11 + i * 11] = 1.0f;
		}


		size = (numberOfFaces * 4 + 2) * 11 * sizeof(GLfloat);
		return vertices;
	}

	GLuint* cylinderIndices(int numberOfFaces, unsigned int& size) {
		static GLuint* indices = new GLuint[numberOfFaces * 4 * 3];

		GLuint face = 2;
		indices[0] = numberOfFaces + 1;
		indices[1] = 1;
		indices[2] = 2;

		for (int i = 3; i < (numberOfFaces) * 3; i += 3) {
			indices[i] = face;
			indices[i + 1] = 1;
			face++;
			indices[i + 2] = face;
		}

		face = 2 + numberOfFaces;
		indices[(numberOfFaces) * 3] = numberOfFaces * 2 + 1;
		indices[(numberOfFaces) * 3 + 1] = 0;
		indices[(numberOfFaces) * 3 + 2] = face;

		for (int i = (numberOfFaces) * 3 + 3; i < (numberOfFaces) * 3 * 2; i += 3) {
			indices[i] = face;
			indices[i + 1] = 0;
			face++;
			indices[i + 2] = face;
		}

		//side faces
		face = 2 + numberOfFaces * 2;
		indices[(numberOfFaces) * 6] = face + numberOfFaces - 1;
		indices[(numberOfFaces) * 6 + 1] = face + numberOfFaces;
		indices[(numberOfFaces) * 6 + 2] = face;

		indices[(numberOfFaces) * 6 + 3] = face + numberOfFaces - 1;
		indices[(numberOfFaces) * 6 + 4] = face + numberOfFaces * 2 - 1;
		indices[(numberOfFaces) * 6 + 5] = face + numberOfFaces;

		//3 i 4 seria scianek, po 3 miejsca na scianke i od raz robie 2 scianki
		for (int i = (numberOfFaces) * 2 * 3 + 6; i < numberOfFaces * 4 * 3; i += 6) {

			indices[i] = face;
			indices[i + 3] = face;
			indices[i + 4] = face + numberOfFaces;

			face++;
			indices[i + 1] = face;
			indices[i + 2] = face + numberOfFaces;
			indices[i + 5] = face + numberOfFaces;
		}


		size = numberOfFaces * 12 * sizeof(GLuint);
		return indices;
	}
};