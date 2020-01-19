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

	GLfloat* getVerticesArray(HardTypeOfObject type, int nrSides, GLfloat radius, GLfloat height, unsigned int& size) {
		switch (type)
		{
		case cylinder:		return cylinderVertices(nrSides, radius, height, size);
		case cone:			return coneVertices(nrSides, radius, height, size);
		case coneCut:		return coneCutVertices(nrSides, radius, height, size);
		case gear:			return gearVertices(nrSides, radius, height, size);
		case backcylinder:	return backcylinderVertices(nrSides, radius, height, size);
		default:			break;
		}
	}

	GLuint* getIndicesArray(HardTypeOfObject type, int nrSides, unsigned int& size) {
		switch (type)
		{
		case cylinder:			return cylinderIndices(nrSides, size);
		case cone:				return coneIndices(nrSides, size);
		case coneCut:			return coneCutIndices(nrSides, size);
		case gear:				return gearIndices(nrSides, size);
		case backcylinder:		return backcylinderIndices(nrSides, size);
		default:				break;
		}
	}

private:
	GLfloat* cylinderVertices(int nrSides, GLfloat radius, GLfloat height, unsigned int& size) {
		static GLfloat* vertices = new GLfloat[(nrSides * 4 + 2) * 11];

		for (int i = 0; i < (nrSides * 4 + 2) * 11; i++) {
			vertices[i] = 0.0f;
		}

		//points 0 and 1
		vertices[1] = height;
		//normal
		vertices[9] = 1.0f;

		vertices[(nrSides + 2) * 11] = radius;
		vertices[(nrSides + 2) * 11 + 1] = height;
		//normal
		vertices[(nrSides + 2) * 11 + 9] = 1.0f;


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

		//point above point nr 2
		vertices[28 + nrSides * 11] = 1.0f;
		vertices[29 + nrSides * 11] = 0.75f;

		//obracanie - macierz
		glm::mat4 rotationZ; //macierz rotacji
		rotationZ = glm::rotate(rotationZ, glm::radians(360.0f / nrSides), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec4 currentV = glm::vec4(radius, 0.0f, 0.0f, 1.0f);//wekor z wierzcholkiem 3d
		glm::vec4 currentTexturePos = glm::vec4(0.25f, 0.0f, 0.0f, 1.0f);//wektor z pozycja wierzcholka na teksturze
		for (int i = 0; i < nrSides - 1; i++) {
			currentV = rotationZ * currentV;
			currentTexturePos = rotationZ * currentTexturePos;

			vertices[33 + i * 11] = currentV[0];
			vertices[35 + i * 11] = currentV[2];
			//normals
			vertices[42 + i * 11] = -1.0f;

			vertices[33 + nrSides * 11 + i * 11] = currentV[0];
			vertices[34 + nrSides * 11 + i * 11] = height;
			vertices[35 + nrSides * 11 + i * 11] = currentV[2];
			//normals
			vertices[42 + nrSides * 11 + i * 11] = 1.0f;

			vertices[39 + i * 11] = currentTexturePos[0] + 0.75f;
			vertices[40 + i * 11] = currentTexturePos[2] + 0.75f;

			vertices[39 + nrSides * 11 + i * 11] = currentTexturePos[0] + 0.75f;
			vertices[40 + nrSides * 11 + i * 11] = currentTexturePos[2] + 0.75f;
		}
		//vertices duplication

		for (int i = 0; i < nrSides; i++) {
			vertices[22 + nrSides * 2 * 11 + i * 11] = vertices[22 + i * 11];
			vertices[24 + nrSides * 2 * 11 + i * 11] = vertices[24 + i * 11];
			//normals
			vertices[30 + nrSides * 2 * 11 + i * 11] = vertices[22 + i * 11];
			vertices[32 + nrSides * 2 * 11 + i * 11] = vertices[24 + i * 11];

			vertices[22 + nrSides * 3 * 11 + i * 11] = vertices[22 + nrSides * 11 + i * 11];
			vertices[23 + nrSides * 3 * 11 + i * 11] = vertices[23 + nrSides * 11 + i * 11];
			vertices[24 + nrSides * 3 * 11 + i * 11] = vertices[24 + nrSides * 11 + i * 11];
			//normals
			vertices[30 + nrSides * 3 * 11 + i * 11] = vertices[22 + nrSides * 11 + i * 11];
			vertices[32 + nrSides * 3 * 11 + i * 11] = vertices[24 + nrSides * 11 + i * 11];

			//texture
			if (i % 2 == 0) {
				vertices[28 + nrSides * 2 * 11 + i * 11] = 0.5f / nrSides * 4;
				vertices[28 + nrSides * 3 * 11 + i * 11] = 0.5f / nrSides * 4;
			}

			vertices[29 + nrSides * 3 * 11 + i * 11] = 1.0f;
		}
		size = (nrSides * 4 + 2) * 11 * sizeof(GLfloat);
		return vertices;
	}

	GLuint* cylinderIndices(int nrSides, unsigned int& size) {
		static GLuint* indices = new GLuint[nrSides * 4 * 3];

		GLuint face = 2;
		indices[0] = nrSides + 1;
		indices[1] = 1;
		indices[2] = 2;

		for (int i = 3; i < (nrSides) * 3; i += 3) {
			indices[i] = face;
			indices[i + 1] = 1;
			face++;
			indices[i + 2] = face;
		}

		face = 2 + nrSides;
		indices[(nrSides) * 3] = nrSides * 2 + 1;
		indices[(nrSides) * 3 + 1] = 0;
		indices[(nrSides) * 3 + 2] = face;

		for (int i = (nrSides) * 3 + 3; i < (nrSides) * 3 * 2; i += 3) {
			indices[i] = face;
			indices[i + 1] = 0;
			face++;
			indices[i + 2] = face;
		}

		//side faces
		face = 2 + nrSides * 2;
		indices[(nrSides) * 6] = face + nrSides - 1;
		indices[(nrSides) * 6 + 1] = face + nrSides;
		indices[(nrSides) * 6 + 2] = face;

		indices[(nrSides) * 6 + 3] = face + nrSides - 1;
		indices[(nrSides) * 6 + 4] = face + nrSides * 2 - 1;
		indices[(nrSides) * 6 + 5] = face + nrSides;

		//3 i 4 seria scianek
		for (int i = (nrSides) * 2 * 3 + 6; i < nrSides * 4 * 3; i += 6) {
			indices[i] = face;
			indices[i + 3] = face;
			indices[i + 4] = face + nrSides;

			face++;
			indices[i + 1] = face;
			indices[i + 2] = face + nrSides;
			indices[i + 5] = face + nrSides;
		}
		size = nrSides * 12 * sizeof(GLuint);
		return indices;
	}

	GLfloat* coneVertices(int nrSides, GLfloat radius, GLfloat height, unsigned int& size) {
		static GLfloat* vertices = new GLfloat[(nrSides + 2) * 11];

		for (int i = 0; i < (nrSides + 2) * 11; i++) {
			vertices[i] = 0.0f;
		}
		//points 0 and 1
		vertices[1] = height;
		//texture
		vertices[6] = 0.5f;
		vertices[7] = 1.0f;
		//normal
		vertices[9] = 1.0f;

		vertices[17] = 0.5f;
		vertices[18] = 1.0f;
		//normal
		vertices[20] = -1.0f;

		//point 2
		vertices[22] = radius;

		//obracanie - macierz
		glm::mat4 rotationZ;
		rotationZ = glm::rotate(rotationZ, glm::radians(360.0f / nrSides), glm::vec3(0.0f, 1.0f, 0.0f));

		//wektor z 2. wierzcho³kiem
		glm::vec4 currentV = glm::vec4(radius, 0.0f, 0.0f, 1.0f);
		for (int i = 0; i < nrSides - 1; i++) {
			currentV = rotationZ * currentV;

			vertices[33 + i * 11] = currentV[0];
			vertices[34 + i * 11] = currentV[1];
			vertices[35 + i * 11] = currentV[2];

			//normals
			vertices[41 + i * 11] = currentV[0];
			vertices[43 + i * 11] = currentV[2];

			//texture
			if (i % 2 == 0)
				vertices[39 + i * 11] = (1.0f * 6) / nrSides;
		}
		size = (nrSides + 2) * 11 * sizeof(GLfloat);
		return vertices;
	}

	GLuint* coneIndices(int nrSides, unsigned int& size) {
		static GLuint* indices = new GLuint[nrSides * 2 * 3];

		GLuint face = 2;
		indices[0] = nrSides + 1;
		indices[1] = 0;
		indices[2] = 2;

		for (int i = 3; i < (nrSides) * 3; i += 3) {
			indices[i] = face;
			indices[i + 1] = 0;
			face++;
			indices[i + 2] = face;
		}

		face = 2;
		indices[(nrSides) * 3] = nrSides + 1;
		indices[(nrSides) * 3 + 1] = 1;
		indices[(nrSides) * 3 + 2] = 2;

		for (int i = (nrSides) * 3 + 3; i < (nrSides) * 3 * 2; i += 3) {
			indices[i] = face;
			indices[i + 1] = 1;
			face++;
			indices[i + 2] = face;
		}

		size = nrSides * 6 * sizeof(GLuint);
		return indices;
	}

	GLfloat* coneCutVertices(int nrSides, GLfloat radius, GLfloat height, unsigned int& size) {
		static GLfloat* vertices = new GLfloat[(nrSides * 4 + 2) * 11];

		for (int i = 0; i < (nrSides * 4 + 2) * 11; i++) {
			vertices[i] = 0.0f;
		}
		GLfloat cut = 0.3f;
		//points 0 and 1
		vertices[1] = height * cut;
		//normal
		vertices[9] = 1.0f;

		vertices[(nrSides + 2) * 11] = radius * (1 - cut);
		vertices[(nrSides + 2) * 11 + 1] = height * cut;
		//normal
		vertices[(nrSides + 2) * 11 + 9] = 1.0f;

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
		vertices[28 + nrSides * 11] = 1.0f;
		vertices[29 + nrSides * 11] = 0.75f;
		//normal
		vertices[31 + nrSides * 11] = 1.0f; //niepotrzebne

		//obracanie - macierz
		glm::mat4 rotationZ; //macierz rotacji
		rotationZ = glm::rotate(rotationZ, glm::radians(360.0f / nrSides), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec4 currentV = glm::vec4(radius, 0.0f, 0.0f, 1.0f);//wekor z wierzcholkiem 3d
		glm::vec4 currentTexturePos = glm::vec4(0.25f, 0.0f, 0.0f, 1.0f);//wektor z pozycja wierzcholka na teksturze
		for (int i = 0; i < nrSides - 1; i++) {
			currentV = rotationZ * currentV;
			currentTexturePos = rotationZ * currentTexturePos;

			vertices[33 + i * 11] = currentV[0];
			vertices[35 + i * 11] = currentV[2];
			//normals
			vertices[42 + i * 11] = -1.0f;

			vertices[33 + nrSides * 11 + i * 11] = currentV[0] * (1 - cut);
			vertices[34 + nrSides * 11 + i * 11] = height * cut;
			vertices[35 + nrSides * 11 + i * 11] = currentV[2] * (1 - cut);
			//normals
			vertices[42 + nrSides * 11 + i * 11] = 1.0f;

			vertices[39 + i * 11] = currentTexturePos[0] + 0.75f;
			vertices[40 + i * 11] = currentTexturePos[2] + 0.75f;

			vertices[39 + nrSides * 11 + i * 11] = currentTexturePos[0] + 0.75f;
			vertices[40 + nrSides * 11 + i * 11] = currentTexturePos[2] + 0.75f;
		}
		//vertices duplication

		for (int i = 0; i < nrSides; i++) {
			vertices[22 + nrSides * 2 * 11 + i * 11] = vertices[22 + i * 11];
			vertices[24 + nrSides * 2 * 11 + i * 11] = vertices[24 + i * 11];
			//normals
			vertices[30 + nrSides * 2 * 11 + i * 11] = vertices[22 + i * 11];
			vertices[32 + nrSides * 2 * 11 + i * 11] = vertices[24 + i * 11];

			vertices[22 + nrSides * 3 * 11 + i * 11] = vertices[22 + nrSides * 11 + i * 11];
			vertices[23 + nrSides * 3 * 11 + i * 11] = vertices[23 + nrSides * 11 + i * 11];
			vertices[24 + nrSides * 3 * 11 + i * 11] = vertices[24 + nrSides * 11 + i * 11];
			//normals
			vertices[30 + nrSides * 3 * 11 + i * 11] = vertices[22 + nrSides * 11 + i * 11];
			vertices[32 + nrSides * 3 * 11 + i * 11] = vertices[24 + nrSides * 11 + i * 11];

			//texture
			if (i % 2 == 0) {
				vertices[28 + nrSides * 2 * 11 + i * 11] = 0.5f / nrSides * 4;
				vertices[28 + nrSides * 3 * 11 + i * 11] = 0.5f / nrSides * 4;
			}
			vertices[29 + nrSides * 3 * 11 + i * 11] = 1.0f;
		}
		size = (nrSides * 4 + 2) * 11 * sizeof(GLfloat);
		return vertices;
	}

	GLuint* coneCutIndices(int nrSides, unsigned int& size) {
		static GLuint* indices = new GLuint[nrSides * 4 * 3];

		GLuint face = 2;
		indices[0] = nrSides + 1;
		indices[1] = 1;
		indices[2] = 2;

		for (int i = 3; i < (nrSides) * 3; i += 3) {
			indices[i] = face;
			indices[i + 1] = 1;
			face++;
			indices[i + 2] = face;
		}

		face = 2 + nrSides;
		indices[(nrSides) * 3] = nrSides * 2 + 1;
		indices[(nrSides) * 3 + 1] = 0;
		indices[(nrSides) * 3 + 2] = face;

		for (int i = (nrSides) * 3 + 3; i < (nrSides) * 3 * 2; i += 3) {
			indices[i] = face;
			indices[i + 1] = 0;
			face++;
			indices[i + 2] = face;
		}

		//side faces
		face = 2 + nrSides * 2;
		indices[(nrSides) * 6] = face + nrSides - 1;
		indices[(nrSides) * 6 + 1] = face + nrSides;
		indices[(nrSides) * 6 + 2] = face;

		indices[(nrSides) * 6 + 3] = face + nrSides - 1;
		indices[(nrSides) * 6 + 4] = face + nrSides * 2 - 1;
		indices[(nrSides) * 6 + 5] = face + nrSides;

		//3 i 4 seria scianek
		for (int i = (nrSides) * 2 * 3 + 6; i < nrSides * 4 * 3; i += 6) {
			if (i == 84 || i == 90) {
				face++;
				continue;
			}
			indices[i] = face;
			indices[i + 3] = face;
			indices[i + 4] = face + nrSides;

			face++;
			indices[i + 1] = face;
			indices[i + 2] = face + nrSides;
			indices[i + 5] = face + nrSides;
		}
		size = nrSides * 12 * sizeof(GLuint);
		return indices;
	}


	GLfloat* gearVertices(int nrSides, GLfloat radius, GLfloat height, unsigned int& size) {
		static GLfloat* vertices = new GLfloat[(nrSides * 4 + 2) * 11];

		for (int i = 0; i < (nrSides * 4 + 2) * 11; i++) {
			vertices[i] = 0.0f;
		}

		//points 0 and 1
		vertices[1] = height;
		//normal
		vertices[9] = 1.0f;

		vertices[(nrSides + 2) * 11] = radius;
		vertices[(nrSides + 2) * 11 + 1] = height;
		//normal
		vertices[(nrSides + 2) * 11 + 9] = 1.0f;

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

		//point above point nr 2
		vertices[28 + nrSides * 11] = 1.0f;
		vertices[29 + nrSides * 11] = 0.75f;

		//obracanie - macierz
		glm::mat4 rotationZ, negRotationZ;
		rotationZ = glm::rotate(rotationZ, glm::radians(360.0f / nrSides), glm::vec3(0.0f, 1.0f, 0.0f));
		negRotationZ = glm::rotate(negRotationZ, (-1.0f) * glm::radians(360.0f / nrSides), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec4 currentV = glm::vec4(radius, 0.0f, 0.0f, 1.0f);				//wekor z wierzcholkiem 3d
		glm::vec4 currentTexturePos = glm::vec4(0.25f, 0.0f, 0.0f, 1.0f);		//wektor z pozycja wierzcholka na teksturze

		glm::vec4 scale = glm::vec4(1.25f, 0.0f, 0.0f, 1.0f);					//wektor pomocniczy do skalowania 

		glm::vec4 scaleBack = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);					//wektor pomocniczy do reskalowania

		int j;
		for (int i = 0; i < nrSides - 1; i++) {

			if (i % 2 == 0) {
				for (j = 0; j < i; j++)
					currentV = negRotationZ * currentV;
				currentV = scale * currentV;
				for (j = 0; j < i; j++)
					currentV = rotationZ * currentV;
			}

			currentV = rotationZ * currentV;
			currentTexturePos = rotationZ * currentTexturePos;

			vertices[33 + i * 11] = currentV[0];
			vertices[35 + i * 11] = currentV[2];
			//normals
			vertices[42 + i * 11] = -1.0f;

			vertices[33 + nrSides * 11 + i * 11] = currentV[0];
			vertices[34 + nrSides * 11 + i * 11] = height;
			vertices[35 + nrSides * 11 + i * 11] = currentV[2];
			//normals
			vertices[42 + nrSides * 11 + i * 11] = 1.0f;

			vertices[39 + i * 11] = currentTexturePos[0] + 0.75f;
			vertices[40 + i * 11] = currentTexturePos[2] + 0.75f;

			vertices[39 + nrSides * 11 + i * 11] = currentTexturePos[0] + 0.75f;
			vertices[40 + nrSides * 11 + i * 11] = currentTexturePos[2] + 0.75f;

			if (i % 2 == 0) {
				for (j = 0; j < i + 1; j++)
					currentV = negRotationZ * currentV;
				currentV = scaleBack * currentV;
				for (j = 0; j < i + 1; j++)
					currentV = rotationZ * currentV;
			}
		}
		//vertices duplication

		for (int i = 0; i < nrSides; i++) {
			vertices[22 + nrSides * 2 * 11 + i * 11] = vertices[22 + i * 11];
			vertices[24 + nrSides * 2 * 11 + i * 11] = vertices[24 + i * 11];
			//normals
			vertices[30 + nrSides * 2 * 11 + i * 11] = vertices[22 + i * 11];
			vertices[32 + nrSides * 2 * 11 + i * 11] = vertices[24 + i * 11];

			vertices[22 + nrSides * 3 * 11 + i * 11] = vertices[22 + nrSides * 11 + i * 11];
			vertices[23 + nrSides * 3 * 11 + i * 11] = vertices[23 + nrSides * 11 + i * 11];
			vertices[24 + nrSides * 3 * 11 + i * 11] = vertices[24 + nrSides * 11 + i * 11];
			//normals
			vertices[30 + nrSides * 3 * 11 + i * 11] = vertices[22 + nrSides * 11 + i * 11];
			vertices[32 + nrSides * 3 * 11 + i * 11] = vertices[24 + nrSides * 11 + i * 11];

			//texture
			if (i % 2 == 0) {
				vertices[28 + nrSides * 2 * 11 + i * 11] = 0.5f / nrSides * 4;
				vertices[28 + nrSides * 3 * 11 + i * 11] = 0.5f / nrSides * 4;
			}

			vertices[29 + nrSides * 3 * 11 + i * 11] = 1.0f;
		}
		size = (nrSides * 4 + 2) * 11 * sizeof(GLfloat);
		return vertices;
	}

	GLuint* gearIndices(int nrSides, unsigned int& size) {
		static GLuint* indices = new GLuint[nrSides * 4 * 3];

		GLuint face = 2;
		indices[0] = nrSides + 1;
		indices[1] = 1;
		indices[2] = 2;

		for (int i = 3; i < (nrSides) * 3; i += 3) {
			indices[i] = face;
			indices[i + 1] = 1;
			face++;
			indices[i + 2] = face;
		}

		face = 2 + nrSides;
		indices[(nrSides) * 3] = nrSides * 2 + 1;
		indices[(nrSides) * 3 + 1] = 0;
		indices[(nrSides) * 3 + 2] = face;

		for (int i = (nrSides) * 3 + 3; i < (nrSides) * 3 * 2; i += 3) {
			indices[i] = face;
			indices[i + 1] = 0;
			face++;
			indices[i + 2] = face;
		}

		//side faces
		face = 2 + nrSides * 2;
		indices[(nrSides) * 6] = face + nrSides - 1;
		indices[(nrSides) * 6 + 1] = face + nrSides;
		indices[(nrSides) * 6 + 2] = face;

		indices[(nrSides) * 6 + 3] = face + nrSides - 1;
		indices[(nrSides) * 6 + 4] = face + nrSides * 2 - 1;
		indices[(nrSides) * 6 + 5] = face + nrSides;

		//3 i 4 seria scianek
		for (int i = (nrSides) * 2 * 3 + 6; i < nrSides * 4 * 3; i += 6) {

			indices[i] = face;
			indices[i + 3] = face;
			indices[i + 4] = face + nrSides;

			face++;
			indices[i + 1] = face;
			indices[i + 2] = face + nrSides;
			indices[i + 5] = face + nrSides;
		}
		size = nrSides * 12 * sizeof(GLuint);
		return indices;
	}

	GLfloat* backcylinderVertices(int nrSides, GLfloat radius, GLfloat height, unsigned int& size) {
		static GLfloat* vertices = new GLfloat[(nrSides * 4 + 2) * 11];

		for (int i = 0; i < (nrSides * 4 + 2) * 11; i++) {
			vertices[i] = 0.0f;
		}

		//points 0 and 1
		vertices[1] = height;
		//normal
		vertices[9] = (-1) * 1.0f;

		vertices[(nrSides + 2) * 11] = radius;
		vertices[(nrSides + 2) * 11 + 1] = height;
		//normal
		vertices[(nrSides + 2) * 11 + 9] = (-1) * 1.0f;

		//texture
		vertices[6] = 0.75f;
		vertices[7] = 0.75f;

		vertices[17] = 0.75f;
		vertices[18] = 0.75f;

		//normal
		vertices[20] = (-1) * (-1.0f);

		//point nr 2
		vertices[22] = radius;
		vertices[28] = 1.0f;
		vertices[29] = 0.75f;

		//normal
		vertices[31] = (-1) * (-1.0f);

		//point above point nr 2
		vertices[28 + nrSides * 11] = 1.0f;
		vertices[29 + nrSides * 11] = 0.75f;

		//obracanie - macierz
		glm::mat4 rotationZ; //macierz rotacji
		rotationZ = glm::rotate(rotationZ, glm::radians(360.0f / nrSides), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec4 currentV = glm::vec4(radius, 0.0f, 0.0f, 1.0f);//wekor z wierzcholkiem 3d
		glm::vec4 currentTexturePos = glm::vec4(0.25f, 0.0f, 0.0f, 1.0f);//wektor z pozycja wierzcholka na teksturze
		for (int i = 0; i < nrSides - 1; i++) {
			currentV = rotationZ * currentV;
			currentTexturePos = rotationZ * currentTexturePos;

			vertices[33 + i * 11] = currentV[0];
			vertices[35 + i * 11] = currentV[2];
			//normals
			vertices[42 + i * 11] = (-1) * (-1.0f);

			vertices[33 + nrSides * 11 + i * 11] = currentV[0];
			vertices[34 + nrSides * 11 + i * 11] = height;
			vertices[35 + nrSides * 11 + i * 11] = currentV[2];
			//normals
			vertices[42 + nrSides * 11 + i * 11] = (-1) * 1.0f;

			vertices[39 + i * 11] = currentTexturePos[0] + 0.75f;
			vertices[40 + i * 11] = currentTexturePos[2] + 0.75f;

			vertices[39 + nrSides * 11 + i * 11] = currentTexturePos[0] + 0.75f;
			vertices[40 + nrSides * 11 + i * 11] = currentTexturePos[2] + 0.75f;
		}

		//vertices duplication

		for (int i = 0; i < nrSides; i++) {
			vertices[22 + nrSides * 2 * 11 + i * 11] = vertices[22 + i * 11];
			vertices[24 + nrSides * 2 * 11 + i * 11] = vertices[24 + i * 11];
			//normals
			vertices[30 + nrSides * 2 * 11 + i * 11] = (-1) * vertices[22 + i * 11];
			vertices[32 + nrSides * 2 * 11 + i * 11] = (-1) * vertices[24 + i * 11];

			vertices[22 + nrSides * 3 * 11 + i * 11] = vertices[22 + nrSides * 11 + i * 11];
			vertices[23 + nrSides * 3 * 11 + i * 11] = vertices[23 + nrSides * 11 + i * 11];
			vertices[24 + nrSides * 3 * 11 + i * 11] = vertices[24 + nrSides * 11 + i * 11];
			//normals
			vertices[30 + nrSides * 3 * 11 + i * 11] = (-1)*vertices[22 + nrSides * 11 + i * 11];
			vertices[32 + nrSides * 3 * 11 + i * 11] = (-1) * vertices[24 + nrSides * 11 + i * 11];

			//texture
			if (i % 2 == 0) {
				vertices[28 + nrSides * 2 * 11 + i * 11] = 0.5f / nrSides * 4;
				vertices[28 + nrSides * 3 * 11 + i * 11] = 0.5f / nrSides * 4;
			}

			vertices[29 + nrSides * 3 * 11 + i * 11] = 1.0f;
		}
		size = (nrSides * 4 + 2) * 11 * sizeof(GLfloat);
		return vertices;
	}

	GLuint* backcylinderIndices(int nrSides, unsigned int& size) {
		static GLuint* indices = new GLuint[nrSides * 4 * 3];

		GLuint face = 2;
		indices[0] = nrSides + 1;
		indices[1] = 1;
		indices[2] = 2;

		for (int i = 3; i < (nrSides) * 3; i += 3) {
			indices[i] = face;
			indices[i + 1] = 1;
			face++;
			indices[i + 2] = face;
		}

		face = 2 + nrSides;
		indices[(nrSides) * 3] = nrSides * 2 + 1;
		indices[(nrSides) * 3 + 1] = 0;
		indices[(nrSides) * 3 + 2] = face;

		for (int i = (nrSides) * 3 + 3; i < (nrSides) * 3 * 2; i += 3) {
			indices[i] = face;
			indices[i + 1] = 0;
			face++;
			indices[i + 2] = face;
		}

		//side faces
		face = 2 + nrSides * 2;
		indices[(nrSides) * 6] = face + nrSides - 1;
		indices[(nrSides) * 6 + 1] = face + nrSides;
		indices[(nrSides) * 6 + 2] = face;

		indices[(nrSides) * 6 + 3] = face + nrSides - 1;
		indices[(nrSides) * 6 + 4] = face + nrSides * 2 - 1;
		indices[(nrSides) * 6 + 5] = face + nrSides;

		//3 i 4 seria scianek
		for (int i = (nrSides) * 2 * 3 + 6; i < nrSides * 4 * 3; i += 6) {

			indices[i] = face;
			indices[i + 3] = face;
			indices[i + 4] = face + nrSides;

			face++;
			indices[i + 1] = face;
			indices[i + 2] = face + nrSides;
			indices[i + 5] = face + nrSides;
		}
		size = nrSides * 12 * sizeof(GLuint);
		return indices;
	}

};