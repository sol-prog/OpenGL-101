// Draw four triangles on a red background
#include <GL/glew.h>
#include <GL/glfw.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
//#include <ctime>
#include <FreeImage.h>

// Read a shader source from a file
// store the shader source in a std::vector<char>
void read_shader_src(const char *fname, std::vector<char> &buffer);

// Compile a shader
GLuint load_and_compile_shader(const char *fname, GLenum shaderType);

// Create a program from two shaders
GLuint create_program(const char *path_vert_shader, const char *path_frag_shader);

// Called when the window is resized
void GLFWCALL window_resized(int width, int height);

// Called for keyboard events
void keyboard(int key, int action);

// Render scene
void display(GLuint &vao);

// Initialize the data to be rendered
void initialize(GLuint &vao);

// Load an image from the disk with FreeImage
void load_image(const char *fname);

// Show the effect of the wrapping options on the current texture
void wrap_tests();

// Global variable for switching through the warpping modes
GLuint wrap_option = 0;

int main () {
	// Initialize GLFW
	if ( !glfwInit()) {
		std::cerr << "Failed to initialize GLFW! I'm out!" << std::endl;
		exit(-1);
	}

	// Use OpenGL 3.2 core profile
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);

	// Open a window and attach an OpenGL rendering context to the window surface
	if( !glfwOpenWindow(800, 600, 8, 8, 8, 0, 0, 0, GLFW_WINDOW)) {
		std::cerr << "Failed to open a window! I'm out!" << std::endl;
		glfwTerminate();
		exit(-1);
	}

	// Register a callback function for window resize events
	glfwSetWindowSizeCallback( window_resized );

	// Register a callback function for keyboard pressed events
	glfwSetKeyCallback(keyboard);	

	// Print the OpenGL version
	int major, minor, rev;
	glfwGetGLVersion(&major, &minor, &rev);
	std::cout << "OpenGL - " << major << "." << minor << "." << rev << std::endl;

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW! I'm out!" << std::endl;
		glfwTerminate();
		exit(-1);
	}

	// Create a vertex array object
	GLuint vao;

	// Initialize the data to be rendered
	initialize(vao);

	// Create a rendering loop
	int running = GL_TRUE;

	while(running) {
		// Display scene
		display(vao);

		// Pool for events
		glfwPollEvents();
		// Check if the window was closed
		running = glfwGetWindowParam(GLFW_OPENED);
	}

	// Terminate GLFW
	glfwTerminate();

	return 0;
}

// Render scene
void display(GLuint &vao) {
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vao);
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Swap front and back buffers
	glfwSwapBuffers();
}

void initialize(GLuint &vao) {
	// Use a Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 1 square (made by 2 triangles) to be rendered
	GLfloat vertices_position[8] = {
		-1.0, -1.0,
		1.0, -1.0,
		1.0, 1.0,
		-1.0, 1.0,
	};

	GLfloat texture_coord[8] = {
		-1.0, -1.0,
		2.0, -1.0,
		2.0, 2.0,
		-1.0, 2.0,
	};

	GLuint indices[6] = {
		0, 1, 2,
		2, 3, 0
	};

	// Create a Vector Buffer Object that will store the vertices on video memory
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Allocate space for vertex positions and texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position) + sizeof(texture_coord), NULL, GL_STATIC_DRAW);

	// Transfer the vertex positions:
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_position), vertices_position);

	// Transfer the texture coordinates:
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices_position), sizeof(texture_coord), texture_coord);

	// Create an Element Array Buffer that will store the indices array:
	GLuint eab;
	glGenBuffers(1, &eab);

	// Transfer the data from indices to eab
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Create a texture
	GLuint texture;
	glGenTextures(1, &texture);

	// Specify that we work with a 2D texture
	glBindTexture(GL_TEXTURE_2D, texture);

	load_image("squirrel.jpg");

	GLuint shaderProgram = create_program("shaders/vert.shader", "shaders/frag.shader");

	// Bind the output names from the fragment program
	glBindFragDataLocation(shaderProgram, 0, "out_color");

	// Get the location of the attributes that enters in the vertex shader
	GLint position_attribute = glGetAttribLocation(shaderProgram, "position");

	// Specify how the data for position can be accessed
	glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable the attribute
	glEnableVertexAttribArray(position_attribute);

	// Texture coord attribute
	GLint texture_coord_attribute = glGetAttribLocation(shaderProgram, "texture_coord");
	glVertexAttribPointer(texture_coord_attribute, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(vertices_position));
	glEnableVertexAttribArray(texture_coord_attribute);

}

void load_image(const char *fname) {

	// active only for static linking
	#ifdef FREEIMAGE_LIB
		FreeImage_Initialise();
	#endif

	FIBITMAP *bitmap;
	// Get the format of the image file
	FREE_IMAGE_FORMAT fif =FreeImage_GetFileType(fname, 0);

	// If the format can't be determined, try to guess the format from the file name
	if(fif == FIF_UNKNOWN) {
		fif = FreeImage_GetFIFFromFilename(fname);
	}

	// Load the data in bitmap if possible
	if(fif != FIF_UNKNOWN && FreeImage_FIFSupportsReading(fif)) {
		bitmap = FreeImage_Load(fif, fname);
	}
	else {
		bitmap = NULL;
	}

	// PROCESS IMAGE if bitmap was successfully initialized
	if(bitmap) {
		unsigned int w = FreeImage_GetWidth(bitmap);
		unsigned int h = FreeImage_GetHeight(bitmap);
		unsigned pixel_size = FreeImage_GetBPP(bitmap);

		// Get a pointer to the pixel data
		BYTE *data = (BYTE*)FreeImage_GetBits(bitmap);

		// Process only RGB and RGBA images
		if(pixel_size == 24) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, (GLvoid*)data);
		}
		else if (pixel_size == 32) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)data);
		}
		else {
			std::cerr << "pixel size = " << pixel_size << " don't know how to process this case. I'm out!" << std::endl;
			exit(-1);
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		wrap_tests();
	}
	else {
		std::cerr << "Unable to load the image file " << fname  << " I'm out!" << std::endl;
		exit(-1);
	}

	// Clean bitmap;
	FreeImage_Unload(bitmap);

	// active only for static linking
	#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
	#endif	
}

void wrap_tests() {
		if (wrap_option == 0) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			std::cout << "Using GL_REPEAT" << std::endl;
		}
		else if(wrap_option == 1) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			std::cout << "Using GL_CLAMP_TO_EDGE" << std::endl;
		}
		else if(wrap_option == 2) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			std::cout << "Using GL_CLAMP_TO_BORDER" << std::endl;
		}
		else if(wrap_option == 3) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			std::cout << "Using GL_MIRRORED_REPEAT" << std::endl;
		}	
}


// Called when the window is resized
void GLFWCALL window_resized(int width, int height) {
	// Use red to clear the screen
	//glClearColor(1, 0, 0, 1);

	// Set the viewport
	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers();
}

// Called for keyboard events
void keyboard(int key, int action) {
	if(key == 'Q' && action == GLFW_PRESS) {
		glfwTerminate();
		exit(0);
	}
	if(key == 'W' && action == GLFW_PRESS) {
		wrap_option++;
		// Cycle through values from 0 to 3
		if(wrap_option > 3) {
			wrap_option = 0;
		}
		wrap_tests();
	}
}

// Read a shader source from a file
// store the shader source in a std::vector<char>
void read_shader_src(const char *fname, std::vector<char> &buffer) {
	std::ifstream in;
	in.open(fname, std::ios::binary);

	if(in.is_open()) {
		// Get the number of bytes stored in this file
		in.seekg(0, std::ios::end);
		size_t length = (size_t)in.tellg();

		// Go to start of the file
		in.seekg(0, std::ios::beg);

		// Read the content of the file in a buffer
		buffer.resize(length + 1);
		in.read(&buffer[0], length);
		in.close();
		// Add a valid C - string end
		buffer[length] = '\0';
	}
	else {
		std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
		exit(-1);
	}
}

// Compile a shader
GLuint load_and_compile_shader(const char *fname, GLenum shaderType) {
	// Load a shader from an external file
	std::vector<char> buffer;
	read_shader_src(fname, buffer);
	const char *src = &buffer[0];

	// Compile the shader
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	// Check the result of the compilation
	GLint test;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
	if(!test) {
		std::cerr << "Shader compilation failed with this message:" << std::endl;
		std::vector<char> compilation_log(512);
		glGetShaderInfoLog(shader, compilation_log.size(), NULL, &compilation_log[0]);
		std::cerr << &compilation_log[0] << std::endl;
		glfwTerminate();
		exit(-1);
	}
	return shader;
}

// Create a program from two shaders
GLuint create_program(const char *path_vert_shader, const char *path_frag_shader) {
	// Load and compile the vertex and fragment shaders
	GLuint vertexShader = load_and_compile_shader(path_vert_shader, GL_VERTEX_SHADER);
	GLuint fragmentShader = load_and_compile_shader(path_frag_shader, GL_FRAGMENT_SHADER);

	// Attach the above shader to a program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Flag the shaders for deletion
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Link and use the program
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	return shaderProgram;
}

