#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "circle.h"
#include "terrain.h"
#include "math.h"
#include "magnet.h"

// Vertex Shader source code
const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vertexColors;

out vec3 v_vertexColors;

void main() {
	v_vertexColors = vertexColors;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

// Fragment Shader source code
const char* fragmentShaderSource = R"(
#version 330 core

in vec3 v_vertexColors;
out vec4 FragColor;

void main() {
	FragColor = vec4(v_vertexColors.r, v_vertexColors.g, v_vertexColors.b, 1.0f);
}
)";

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void genCircleVertices(GLfloat v[], GLfloat x, GLfloat y, GLfloat radius, int numTriangles);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //uncomment this statement to fix compilation on OS X
#endif
	// glfw window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2D Physics Engine", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	// glad: load all OpenGL function pointers
	// ---------------------------------------

	gladLoadGL();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object & Compile
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Create Shader Program Object and get its reference
	GLuint shaderProgram = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(shaderProgram);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	Circle main;
	main.loadCircle();

	Terrain t;
	t.loadTerrain();

	Magnet m;
	m.loadMagnet();


	GLfloat deltatime = 0.0f;
	GLfloat lastFrame = 0.0f;

	bool isCollision = false;
	GLfloat current_slope = 0;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);

		// Draw Objects & Environment
		main.drawCircle();
		t.drawTerrain();
		m.drawMagnet();

		// Calculate delta time
		// -------------------------------------------------------------------------------
		GLfloat currentFrame = glfwGetTime();
		deltatime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Handle Key Presses
		// -------------------------------------------------------------------------------
		int keyW = glfwGetKey(window, GLFW_KEY_W);
		int keyA = glfwGetKey(window, GLFW_KEY_A);
		int keyD = glfwGetKey(window, GLFW_KEY_D);

		t.update(main.L.center[0]);
		std::vector<GLfloat> m_energy = m.evaluateEnergy(main.L.center[0], main.L.center[1]);

		main.L.LGRNGN = sqrt((main.L.xvel*main.L.xvel) + (main.L.yvel*main.L.yvel)) 
						- 2 * g_const * (main.L.center[0] + 1.0f);

		if (keyW || keyA || keyD) {
			if (keyW) main.adjustVertexData(0.0f, 0.01f);
			if (keyA) main.adjustVertexData(-0.01f, 0.00f);
			if (keyD) main.adjustVertexData(0.01f, 0.00f);

			main.L.yvel = 0;
			main.L.xvel = 0;
			isCollision = false;

		} else if (!main.detectCollision(t.T.t_proj[0], t.T.t_proj[1])) {

			main.simulateMotion(deltatime, (-1.0 * g_const) + m_energy[1], m_energy[0]);

		} else if (!isCollision) { 

			isCollision = true;
			main.L.yvel = 0;
			main.L.xvel = 0;
			main.simulateMotion(deltatime, 0, 0);
			current_slope = t.T.slope[1]/t.T.slope[0];

		} else {
			// Basically rying to define static equilibrium
			if ((current_slope) * (t.T.slope[1]/t.T.slope[0]) > 0) {
				// -dU/dx = mgsin(theta) => a = gsin(theta)
				GLfloat acc = g_const * sin(atan(t.T.slope[1]/t.T.slope[0]));
				GLfloat acc_y = -1.0f * abs(acc) * t.T.slope[1];
				GLfloat acc_x = -1.0f * abs(acc) * t.T.slope[0];
				main.simulateMotion(deltatime, acc_y, acc_x);
			} 
		}
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// glfw: terminate, clearing all previously allocated GLFWresources.
	//---------------------------------------------------------------
	main.delObjects();
	m.delObjects();
	glDeleteProgram(shaderProgram);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
