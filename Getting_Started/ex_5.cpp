// Open a window with GLFW, check default OpenGL version, set background color red
// Register callback functions for GLFW errors, key pressed and window resized events
// Require minimum OpenGL 3.2 core profile and remove the fixed pipeline functionality
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

// Define a few callback functions:
void window_resized(GLFWwindow* window, int width, int height);
void key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods);
void show_glfw_error(int error, const char* description);


int main () {
	// Register the GLFW error callback function
	glfwSetErrorCallback(show_glfw_error);

	// Initialize GLFW
	if ( !glfwInit()) {
		std::cerr << "Failed to initialize GLFW! I'm out!" << '\n';
		exit(-1);
	}

	// Require minimum OpenGL 3.2 core profile and remove the fixed pipeline functionality
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);	

	// Open a window and attach an OpenGL context to the window surface
	GLFWwindow* window = glfwCreateWindow(600, 600, "OpenGL 101", NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to open a window! I'm out!" << '\n';
		glfwTerminate();
		exit(-1);		
	}

	// Set the window context current
	glfwMakeContextCurrent(window);

	// Register the GLFW  window resized callback function
	glfwSetWindowSizeCallback(window, window_resized);

	// Register the GLFW  window key pressed callback function
	glfwSetKeyCallback(window, key_pressed);


	// Set the swap interval, 1 will use your screen refresh rate (vsync)
	glfwSwapInterval(1);

	// Optionally, setup an OpenGL extension loader library
	GLenum err = glewInit();
	if(err != GLEW_OK) {
		std::cerr << "GLEW failed to initialize with error: " << glewGetErrorString(err) <<  '\n';
		glfwTerminate();
		exit(-1);		
	}

	// After this point you can start using any available OpenGL function
	// ==================================================================

	// Print the OpenGL version currently enabled on your machine
	std::cout << glGetString(GL_VERSION) << '\n';

	// Print the available OpenGL extensions 
	// This code should generate a compiler error on Windows, Linux and macOS, because the function 
	// "glGetStringi" is not loaded by default
	// In order to get rid of the above error we need to write our own version or use an OpenGL extension loader library
	// like Glew, Glad ...
	int nr_extensions = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &nr_extensions);

	for(int i = 0; i < nr_extensions; ++i) {
		std::cout << glGetStringi(GL_EXTENSIONS, i) << '\n';
	}

	// Use red to clear the screen
	glClearColor(1, 0, 0, 1);

	// Create a rendering loop that runs until the window is closed
	while (!glfwWindowShouldClose(window)) {
		// Clear the screen (window background)
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw code here ...

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

void show_glfw_error(int error, const char* description) {
	std::cerr << "Error: " << description << '\n';
}

void window_resized(GLFWwindow* window, int width, int height) {
	std::cout << "Window resized, new window size: " << width << " x " << height << '\n';
}

void key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == 'Q' && action == GLFW_PRESS) {
		glfwTerminate();
		exit(0);
	}
}
