/*
Circle Class Header — contains all positioning & rendering information
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Constants
#define PI 3.1415f // or ~3 in engineering speak
#define nTriangles 32 // how smooth do you want the circle to be?
#define g_const 0.981f // Gravity but 1/10 as much b/c we are in a simulation, we are in the matrix, and only the nonchalant sigma will set us free

// Putting this here
#ifndef LAGRANGIAN_H
#define LAGRANGIAN_H

// The mass of our balls will be 2 kg (ideal b/c our KE will just be v^2) 
struct LAGRANGIAN {
    GLfloat xvel; // contains info about direction
    GLfloat yvel; // contains info about direction
    GLfloat LGRNGN; // Kinetic - Potential Energy
    std::vector<GLfloat> center;
};

#endif

class Circle {
    public:
    	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	    GLuint VAO, VBO, VBO2;
        GLfloat vertices[nTriangles * 3];

        //will convert this to init() later
        struct LAGRANGIAN L;

        // Methods
        // Loads Le Circle — il explique soi-même 
        void loadCircle();
        // Dessiner Le Circle — il explique soi-même 
        void drawCircle();
        // Supprimer Le Circle - il explique soi-même
        void delObjects();
        // Créer les vertices de circle - il explique soi-même
        void genCircleVertices(GLfloat v[], GLfloat x, GLfloat y, GLfloat radius);
        // Okay imma stop writing in french now
        void adjustVertexData(GLfloat xoffset, GLfloat yoffset);
        void simulateMotion(GLfloat dtime, GLfloat yacc, GLfloat xacc);
        // Detects collisions with other objects
        bool detectCollision(GLfloat x, GLfloat y);
};
