/*
Circle Class — contains all positioning & rendering information
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "magnet.h"

// Loads Le Circle — il explique soi-même 
void Magnet::loadMagnet() {
	//GLfloat test[nTriangles * 3];
	genVertices(vertices, -0.65f, -0.35f);

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
        0.2f, 0.0f, 1.0f,
        0.2f, 0.0f, 1.0f,
        0.2f, 0.0f, 1.0f,
		0.2f, 0.0f, 1.0f
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
}

// Dessiner Le Circle — il explique soi-même 
void Magnet::drawMagnet() {
    glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void Magnet::delObjects() {
    // Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Magnet::genVertices(GLfloat v[], GLfloat x, GLfloat y) {
	v[0] = x - 0.15f;
	v[1] = y - 0.15f;
	v[2] = 0.0f;

	v[3] = x - 0.15f;
	v[4] = y + 0.15f;
	v[5] = 0.0f;

	v[6] = x + 0.15f;
	v[7] = y + 0.15f;
	v[8] = 0.0f;

	v[9] = x + 0.15f;
	v[10] = y - 0.15f;
	v[11] = 0.0f;
}

// function to find the inverse square root - I saw the opportunity to use it, and I did —— thx xkcd
GLfloat q_rsqrt2(float number)
{
  long i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = number * 0.5F;
  y  = number;
  i  = * ( long * ) &y;                       // evil floating point bit level hacking
  i  = 0x5f3759df - ( i >> 1 );               // what the frick?
  y  = * ( float * ) &i;
  y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
  // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

  return (GLfloat) y;
}

std::vector<GLfloat> Magnet::evaluateEnergy(GLfloat x, GLfloat y) {
	GLfloat magn_x = -0.65f - x;
	GLfloat magn_y = -0.35f - y;
	GLfloat dist = (magn_x*magn_x + magn_y*magn_y);
	GLfloat magn_f = 1.0f/dist;

	GLfloat norm = q_rsqrt2(magn_x*magn_x + magn_y*magn_y);

	std::vector<GLfloat> m_vect = {magn_f * norm * (magn_x), magn_f * norm * (magn_y), magn_f};

	printf("%.6f,", m_vect[0]);
	printf("%.6f\n", m_vect[1]);
	return m_vect;
}