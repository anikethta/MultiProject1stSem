/*
Circle Class — contains all positioning & rendering information
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "circle.h"

// Loads Le Circle — il explique soi-même 
void Circle::loadCircle() {
	//GLfloat test[nTriangles * 3];
	genCircleVertices(vertices, 0, 0.5, 0.1);

	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	// Again, hardcoding this temporarily, just for simplicity
	const std::vector<GLfloat> vertexColors = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
    };

    // Setting Up Colors
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, vertexColors.size() * sizeof(GL_FLOAT), vertexColors.data(), GL_STREAM_DRAW);

    // Linking up attributes in VAO
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(1);

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    // TODO: move this to an init() function
    L.center = {0, 0.5};
	L.xvel = 0;
	L.yvel = 0;
}

// Dessiner Le Circle — il explique soi-même 
void Circle::drawCircle() {
    glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, nTriangles);
}

void Circle::delObjects() {
    // Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Circle::genCircleVertices(GLfloat v[], GLfloat x, GLfloat y, GLfloat radius) {
	GLfloat twicepi = 2.0f * PI;

	for (int i = 0; i < (3 * nTriangles); i += 3) {
		v[i] = x + (radius * cos((i/3) *  twicepi / nTriangles));
	    v[i+1] = y + (radius * sin((i/3) * twicepi / nTriangles));
		v[i+2] = 0;
	}
}

void Circle::adjustVertexData(GLfloat xoffset, GLfloat yoffset) {
    for (int i = 0; i < (3 * nTriangles); i += 3) {
        vertices[i] += xoffset;
        vertices[i+1] += yoffset;
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	L.center[0] += xoffset;
	L.center[1] +=  yoffset;
}

void Circle::simulateMotion(GLfloat dtime, GLfloat yacc, GLfloat xacc) {
    L.yvel += yacc * dtime;
	L.xvel += xacc * dtime;

    adjustVertexData(L.xvel*dtime, L.yvel*dtime);
}

GLfloat distance(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
	return (GLfloat) sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

bool Circle::detectCollision(GLfloat x, GLfloat y) {
	for (int i = 0; i < nTriangles*3; i += 3) {
		if ((abs(vertices[i] - x) <= 0.02f) && (abs(vertices[i+1] - y) <= 0.02f)) {
			return true;
		}
	}

	return false;
}