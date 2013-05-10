// Open a window with GLFW
#include <GL/glfw.h>
#include <cstdlib>
#include <iostream>

int main () {
	// Initialize GLFW
	if ( !glfwInit()) {
		std::cerr << "Failed to initialize GLFW! I'm out!" << std::endl;
		exit(-1);
	}

	// Open a window and attach an OpenGL rendering context to the window surface
	if( !glfwOpenWindow(800, 600, 8, 8, 8, 0, 0, 0, GLFW_WINDOW)) {
		std::cerr << "Failed to open a window! I'm out!" << std::endl;
		glfwTerminate();
		exit(-1);
	}

	// Use red to clear the screen
	glClearColor(1, 0, 0, 1);

	// Create a rendering loop
	int running = GL_TRUE;
	while(running) {
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap front and back buffers
		glfwSwapBuffers();

		// Check if the window was closed
		running = glfwGetWindowParam(GLFW_OPENED);
	}

	// Terminate GLFW
	glfwTerminate();

	return 0;
}
