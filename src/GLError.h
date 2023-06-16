#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) glClearError();\
	x;\
	ASSERT(glCheckError(#x, __FILE__, __LINE__))

static void glClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
// This function helps us handle opengl error in a more simple way
// it uses macro to call the actual opengl function and check it's errors
static bool glCheckError(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << function
			<< " " << file << ":" << line << std::endl;
		return false;
	}

	return true;
}
