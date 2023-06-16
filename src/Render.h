#pragma once
#include "Pong.h"
#include "GL/glew.h"
#include "freeglut.h"
#include <ctime>
#include "GLError.h"
#include <irrKlang.h>

// Create a global pong game object
mjf::Pong pong{ {{-0.8f, 0.3f}, 0.1f, 0.3f}, {{0.8f, 0.3f}, 0.1f, 0.3f}, {{0.0f, 0.0f}, 0.05f, 0.05f} };

// Set game's timer
int g_minute{ 1 };
int g_second{ 20 };
// Check if time is up or not
bool g_timesup{ false };

// Sound stuff
using namespace irrklang;
ISoundEngine* g_soundEngine = createIrrKlangDevice();

void renderTexts()
{
	// Disable the scissor because we want to clear the entire screen
	glDisable(GL_SCISSOR_TEST);

	// Set the color for cleaning operation.
	// This color is saved to color buffer.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Clear the screen.
	glClear(GL_COLOR_BUFFER_BIT);

	std::string leftScore = std::to_string(pong.getLeftScore());
	std::string rightScore = std::to_string(pong.getRightScore());
	std::string txt = "JUST LIKE THE OLD DAYS ...";

	// Set the position of the text
	glRasterPos2f(-0.4, 0.85);
	// Render the left score text
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)leftScore.c_str());

	// Set the position of the text
	glRasterPos2f(0.4, 0.85);
	// Render right score text
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)rightScore.c_str());

	// Set the position of the text
	glRasterPos2f(-0.25, -0.95);
	// Render the poet :)
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char*)txt.c_str());

	// Format timer string
	std::string timer = std::to_string(g_minute) + ":";
	if (g_second >= 10) timer += std::to_string(g_second);
	else timer += "0" + std::to_string(g_second);

	// Set the position of the text
	GLCall(glRasterPos2f(-0.07f, 0.85f));
	// Render timer text
	GLCall(glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)timer.c_str()));

	// We need to render the same texts on the other buffer
	glutSwapBuffers();

	glClear(GL_COLOR_BUFFER_BIT);

	// Render left score
	glRasterPos2f(-0.4, 0.85);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)leftScore.c_str());

	// Render right score
	glRasterPos2f(0.4, 0.85);

	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)rightScore.c_str());

	// Render just like the old days :)
	glRasterPos2f(-0.25, -0.95);

	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char*)txt.c_str());

	// Render timer
	GLCall(glRasterPos2f(-0.07f, 0.85f));
	GLCall(glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)timer.c_str()));
}

void renderEndGame()
{
	// Disable the scissor because we want to clear the entire screen
	glDisable(GL_SCISSOR_TEST);
	// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	std::string txt{"TIME'S UP!"};

	// Set the position of the text
	GLCall(glRasterPos2f(-0.25f, 0.0f));
	// Render the text
	GLCall(glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)txt.c_str()));

	glutSwapBuffers();
}

bool checkTime()
{
	static time_t now{ time(0) };

	if (time(0) - now < 1)
		return true;

	now = time(0);
	g_second -= 1;

	if (g_second < 0)
	{
		g_minute -= 1;
		g_second = 59;
	}
	if (g_minute < 0)
	{
		std::cout << "Times up!" << std::endl;
		g_timesup = true;
		renderEndGame();
		g_soundEngine->drop();
		return false;
	}

	renderTexts();

	return true;
}

void display()
{
	if (g_timesup) return;

	static int leftScore{ 0 };
	static int rightScore{ 0 };

	// Check if scores are changed then render them again
	if (pong.getLeftScore() != leftScore || pong.getRightScore() != rightScore)
	{
		leftScore = pong.getLeftScore();
		rightScore = pong.getRightScore();
		renderTexts();
	}


	if (!checkTime())
		return;

	// Because of performance issues we can not clear the entire screen just to draw some text
	// so we use scissor to clear only a portion of the screen.
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 35, 500, 420);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	// Update the game logic everytick
	updateLeftPaddle(&pong);
	updateRightPaddle(&pong);
	updateBall(&pong);

	if (pong.isBallOutsideTheWindow())
		pong.respawnBall();

	// Copy left paddle array to the buffer objects
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, pong.getLeftPaddleArr(), GL_DYNAMIC_COPY));
	// Draw left paddle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// Copy right paddle array to the buffer objects
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, pong.getRightPaddleArr(), GL_DYNAMIC_COPY));
	// Draw right paddle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// Copy ball array to the buffer object
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, pong.getBallArr(), GL_DYNAMIC_COPY));
	// Draw the array
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// Swap buffers to see the changes
	glutSwapBuffers();
}
