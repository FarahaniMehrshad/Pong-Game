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
	glGenBuffers(1, &buffer);
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));

	ShaderProgramSource shaders{ParseShader("res/shaders/Basic.shaders")};

	GLuint vertexShader{glCreateShader(GL_VERTEX_SHADER)};
	GLuint fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};

	const char* vsrc = shaders.VertexSource.c_str();
	const char* fsrc = shaders.FragmentSource.c_str();

	glShaderSource(vertexShader, 1, &vsrc, nullptr);
	glShaderSource(fragmentShader, 1, &fsrc, nullptr);
	glCompileShader(vertexShader);

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

	glCompileShader(fragmentShader);
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

	GLuint program{glCreateProgram()};
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glUseProgram(program);

	GLint location{ glGetAttribLocation(program, "position") };

	GLCall(glEnableVertexAttribArray(location));

	GLCall(glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0));
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitContextVersion(3, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);

	int window{ glutCreateWindow("PONG GAME") };

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	OpenGLInit();

	renderTexts();

	glutIdleFunc(display);

	glutKeyboardFunc(keyboardHandler);
	glutKeyboardUpFunc(keyboardUpHandler);

	glutDisplayFunc(display);

	g_soundEngine->play2D("pong.mp3", true);

	glutMainLoop();

	delete g_soundEngine;

	return 0;
}