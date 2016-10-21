// Open a window with GLFW, check default OpenGL version, set background color red
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

int main () {
	// Initialize GLFW
	if ( !glfwInit()) {
		std::cerr << "Failed to initialize GLFW! I'm out!" << '\n';
		exit(-1);
	}

	// Open a window and attach an OpenGL context to the window surface
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL 101", NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to open a window! I'm out!" << '\n';
		glfwTerminate();
		exit(-1);		
	}

	// Set the window context current
	glfwMakeContextCurrent(window);

	// Set the swap interval, 1 will use your screen refresh rate (vsync)
	glfwSwapInterval(1);

	// Optionally, setup an OpenGL extension loader library
	// ...

	// After this point you can start using any available OpenGL function
	// ==================================================================

	// Print the OpenGL version currently enabled on your machine
	std::cout << glGetString(GL_VERSION) << '\n';

	// Use red to clear the screen
	glClearColor(1, 0, 0, 1);

	// Create a rendering loop that runs until the window is closed
	while (!glfwWindowShouldClose(window)) {
		// Clear the screen (window background)
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw
		// ...

		// Swap front and back buffers for the current window
		glfwSwapBuffers(window);

		// Poll for events
		glfwPollEvents();
	}

	// Destroy the window and its context
	glfwDestroyWindow(window);

	// Terminate GLFW
	glfwTerminate();
	return 0;
}
