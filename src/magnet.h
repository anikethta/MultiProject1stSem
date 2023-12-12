/*
Circle Class Header — contains all positioning & rendering information
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define charge_const 2.0f;

class Magnet {
    public:
    	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	    GLuint VAO, VBO, VBO2;
        GLfloat vertices[12];

        // Methods
        void loadMagnet();
        void drawMagnet();
        void delObjects();
        void genVertices(GLfloat v[], GLfloat x, GLfloat y);
        std::vector<GLfloat> evaluateEnergy(GLfloat x, GLfloat y);
};
