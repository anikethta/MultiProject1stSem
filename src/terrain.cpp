#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "terrain.h"

void Terrain::loadTerrain() {
	generateVertices(vertices);

	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO2);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

    const std::vector<GLfloat> vertexColors = {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
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

void Terrain::drawTerrain() {
    glBindVertexArray(VAO2);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 11);
}

// Hard Coding The Terrain Temporarily — this is ugly as hell
void Terrain::generateVertices(GLfloat v[]) {
    v[0] = -1.0f;
    v[1] = -1.0f;
    v[2] = 0.0f;

    v[3] = -0.8f;
    v[4] = -0.5f;
    v[5] = 0.0f;

    v[6] = -0.8f;
    v[7] = -1.0f;
    v[8] = 0.0f;

    v[9] = -0.5f;
    v[10] = -0.5f;
    v[11] = 0.0f;

    v[12] = -0.5f;
    v[13] = -1.0f;
    v[14] = 0.0f;

    v[15] = -0.3f;
    v[16] = -0.9f;
    v[17] = 0.0f;

    v[18] = -0.3f;
    v[19] = -1.0f;
    v[20] = 0.0f;

    v[21] = 0.7f;
    v[22] = -0.6f;
    v[23] = 0.0f;

    v[24] = 0.7f;
    v[25] = -1.0f;
    v[26] = 0.0f;

    v[27] = 0.9f;
    v[28] = -0.8f;
    v[29] = 0.0f;

    v[30] = 1.0f;
    v[31] = -1.0f;
    v[32] = 0.0f;
}

// function to find the inverse square root - I saw the opportunity to use it, and I did —— thx xkcd
GLfloat q_rsqrt(float number)
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

std::vector<GLfloat> Terrain::check(GLfloat x) {
    std::vector<GLfloat> output = {0,0,0,0};
    for (int i = 0; i < 33; i+=3) {
        if (x < vertices[i] && i > 6) {
            GLfloat del_y = vertices[i+1] - vertices[i-5];
            GLfloat del_x = vertices[i] - vertices[i-3];
            GLfloat magn = q_rsqrt(del_y*del_y + del_x*del_x);
            T.slope = {magn * del_x, magn * del_y};
            // The y coordinate is literally derived through y = mx + b
            T.t_proj = {x, vertices[i-5] + (del_y/del_x) * (x - vertices[i-3]), magn * del_x, magn * del_y};
            output = {x, vertices[i-5] + (del_y/del_x) * (x - vertices[i-3]), magn * del_x, magn * del_y};
            return output;
        }
    }

    return output;
}

void Terrain::update(GLfloat x) {
    std::vector<GLfloat> d = check(x);
    T.slope = {d[2], d[3]};
    T.t_proj = {d[0], d[1]};
}






