/*
Circle Class Header — contains all terrain generation information
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Putting this here
#ifndef T_H
#define T_H

// The mass of our balls will be 2 kg (ideal b/c our KE will just be v^2) 
struct TData {
    std::vector<GLfloat> slope; // Unit Vector 
    std::vector<GLfloat> t_proj; // Basically the spot on the terrain directly under our circle(s)
};

#endif

class Terrain {
    public:
    	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	    GLuint VAO2, VBO, VBO2;
        GLfloat vertices[33];

        // Should probably rename this later
        struct TData T;

        // Methods
        // Loads Le Terre
        void loadTerrain();

        // Draws Le Terre
        void drawTerrain();

        // Generate Vertices -- going to hardcode this for a little bit
        void generateVertices(GLfloat v[]);

        // Local Linearization at a certain x value
        // Updating our struct T
        std::vector<GLfloat> check(GLfloat x);

        // check(), but Updating our struct T
        void update(GLfloat x);
};
