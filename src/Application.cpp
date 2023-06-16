#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Shader.h"
#include "GLError.h"
#include "Rect.h"
#include "Pong.h"
#include "Interface.h"
#include "freeglut.h"
#include "glut.h"
#include "Render.h"

void OpenGLInit()
{
	GLuint buffer{};
	// Create buffer object
	glGenBuffers(1, &buffer);
	// Bind the buffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));

	// Read shaders from sources and copy them into the shaders struct.
	ShaderProgramSource shaders{ParseShader("res/shaders/Basic.shaders")};

	// Create vertex shader object
	GLuint vertexShader{glCreateShader(GL_VERTEX_SHADER)};
	// Create fragment shader object
	GLuint fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};

	// Convert shaders from std::string to c char
	const char* vsrc = shaders.VertexSource.c_str();
	const char* fsrc = shaders.FragmentSource.c_str();

	// Copy vertex shader source to vertex shader object
	glShaderSource(vertexShader, 1, &vsrc, nullptr);
	// Copy fragment shader souce to fragment shader object
	glShaderSource(fragmentShader, 1, &fsrc, nullptr);
	// Compile vertex shader
	glCompileShader(vertexShader);

	// Check for vertex shader compilation errors
	GLint compiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint logSize{};
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetShaderInfoLog(vertexShader, logSize, NULL, logMsg);
		std::cout << logMsg << std::endl;
		delete[] logMsg;
		std::cout << "ERROR!!!" << std::endl;
	}

	// Compile fragment shader
	glCompileShader(fragmentShader);
	// Check for fragment shader compilation errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint logSize{};
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetShaderInfoLog(fragmentShader, logSize, NULL, logMsg);
		std::cout << logMsg << std::endl;
		delete[] logMsg;
		std::cout << "ERROR!!!" << std::endl;
	}

	// Create a program object
	GLuint program{glCreateProgram()};
	// Attach vertex shader to the program
	glAttachShader(program, vertexShader);
	// Attach fragment shader to the program
	glAttachShader(program, fragmentShader);
	// Link program. Shaders and other stuff need to be linked after compiled and attachd to the program.
	glLinkProgram(program);
	// Tell opengl to use this program and it's shaders.
	glUseProgram(program);
	// Get "position" attribute location.
	GLint location{ glGetAttribLocation(program, "position") };
	// Enable the "position" attribute based on it's location
	GLCall(glEnableVertexAttribArray(location));
	// Tell opengl how to copy data from buffer object to the "position" attribute
	GLCall(glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0));
}

int main(int argc, char** argv)
{
	// Initialize glut
	glutInit(&argc, argv);
	// Set color mode to rgb. Set buffer to double buffer.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	// Specify OpenGL version
	glutInitContextVersion(3, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// Specify window position and size
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);

	// Create the window with the given title
	int window{ glutCreateWindow("PONG GAME") };
	// Check if initializtion failed
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	// Create buffer object, program object and link shaders.
	OpenGLInit();
	
	// Render scores and other text on screen
	renderTexts();

	// Setup idle call back function. this function is called everytick.
	glutIdleFunc(display);

	// Setup keyboard callback function.
	glutKeyboardFunc(keyboardHandler);
	glutKeyboardUpFunc(keyboardUpHandler);

	// Setup the display callback function.
	// this function calls when windows is created or when it is moved.
	glutDisplayFunc(display);

	// Play main game sound
	g_soundEngine->play2D("pong.mp3", true);

	// Call the glut main loop.
	// Events are checked in this loop everytick.
	glutMainLoop();

	delete g_soundEngine;

	return 0;
}