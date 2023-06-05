#pragma once
#include "Pong.h"
#include "GL/glew.h"
#include "freeglut.h"
#include <ctime>
#include "GLError.h"
#include <irrKlang.h>

mjf::Pong pong{ {{-0.8f, 0.3f}, 0.1f, 0.3f}, {{0.8f, 0.3f}, 0.1f, 0.3f}, {{0.0f, 0.0f}, 0.05f, 0.05f} };

int g_minute{ 1 };
int g_second{ 20 };
bool g_timesup{ false };
using namespace irrklang;
ISoundEngine* g_soundEngine = createIrrKlangDevice();

void renderTexts()
{
	glDisable(GL_SCISSOR_TEST);

	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	std::string leftScore = std::to_string(pong.getLeftScore());
	std::string rightScore = std::to_string(pong.getRightScore());
	std::string txt = "JUST LIKE THE OLD DAYS ...";

	// Render left score
	glRasterPos2f(-0.4, 0.85);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)leftScore.c_str());


	// Render right score
	glRasterPos2f(0.4, 0.85);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)rightScore.c_str());

	// Render just like the old days :)
	glRasterPos2f(-0.25, -0.95);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, (const unsigned char*)txt.c_str());

	std::string timer = std::to_string(g_minute) + ":";
	if (g_second >= 10) timer += std::to_string(g_second);
	else timer += "0" + std::to_string(g_second);

	// Render timer
	GLCall(glRasterPos2f(-0.07f, 0.85f));
	GLCall(glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)timer.c_str()));

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
	glDisable(GL_SCISSOR_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	std::string txt{"TIME'S UP!"};

	GLCall(glRasterPos2f(-0.25f, 0.0f));
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

	if (pong.getLeftScore() != leftScore || pong.getRightScore() != rightScore)
	{
		leftScore = pong.getLeftScore();
		rightScore = pong.getRightScore();
		renderTexts();
	}

	if (!checkTime())
		return;

	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 35, 500, 420);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	updateLeftPaddle(&pong);
	updateRightPaddle(&pong);
	updateBall(&pong);

	if (pong.isBallOutsideTheWindow())
		pong.respawnBall();

	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, pong.getLeftPaddleArr(), GL_DYNAMIC_COPY));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, pong.getRightPaddleArr(), GL_DYNAMIC_COPY));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, pong.getBallArr(), GL_DYNAMIC_COPY));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glutSwapBuffers();
}
