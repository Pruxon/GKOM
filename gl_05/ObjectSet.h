#pragma once

#include <GL/GL.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Object.h"
#include "Camera.h"

using namespace std;

class Fence {
	Object *pillar1, *pillar2, *pillar3, *bar;

public:
	Fence(GLfloat length, GLfloat heigth, GLfloat x, GLfloat y, GLfloat z, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation) {
		pillar1 = new Object(cylinder, "wood1.png", 18, 0.05f, heigth, -length / 5 - 3*x, 0.0f + y, -0.1f+z, 0.0f, 0.0f, 0.0f);
		pillar2 = new Object(cylinder, "wood1.png", 18, 0.05f, heigth, (-length / 5) * 4 + x, 0.0f + y, -0.1f + z, 0.0f, 0.0f, 0.0f);
		pillar3 = new Object(cylinder, "wood1.png", 18, 0.05f, heigth, -length / 5 - 0.5*x, 0.0f + y, -0.1f + z, 0.0f, 0.0f, 0.0f);
		bar = new Object(cylinder, "wood1.png", 18, 0.06f, length*3, 0.0f - 3*x, 0.45f + y, 0.0f + z, 0.0f, 0.0f, 90.0f);
	}

	~Fence() {
		delete pillar1;
		delete pillar2;
		delete bar;
	}

	void draw(int programId, Camera camera, int screenWidth, int screenHeigth) {
		pillar1->draw(programId, camera, screenWidth, screenHeigth);
		pillar2->draw(programId, camera, screenWidth, screenHeigth);
		pillar3->draw(programId, camera, screenWidth, screenHeigth);
		bar->draw(programId, camera, screenWidth, screenHeigth);
	}
};


class Tree {
	Object* trunk, * leaves;
public:
	Tree(GLfloat rad, GLfloat height, GLfloat x, GLfloat y, GLfloat z, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation) {
		trunk = new Object(cylinder, "wood1.png", 18, rad*0.15, height*0.15, 0.0f + x, 0.0f + y, 0.0f + z, 0.0f, 0.0f, 0.0f);
		leaves = new Object(cone, "leaves.jpg", 6, rad , height, 0.0f + x, height*0.15 + y, 0.0f + z, 0.0f, 0.0f, 0.0f);
	}

	~Tree() {
		delete trunk;
		delete leaves;
	}

	void draw(int programId, Camera camera, int screenWidth, int screenHeigth) {
		trunk->draw(programId, camera, screenWidth, screenHeigth);
		leaves->draw(programId, camera, screenWidth, screenHeigth);
	}
};