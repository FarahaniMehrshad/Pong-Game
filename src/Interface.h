#pragma once
#include "Pong.h"
#include "freeglut.h"

bool g_is_w_pressed{ false };
bool g_is_s_pressed{ false };
bool g_is_i_pressed{ false };
bool g_is_k_pressed{ false };

float g_ball_xSpeed{ 0.0005f };
float g_ball_ySpeed{ 0.0f };

void updateLeftPaddle(mjf::Pong* pong)
{
	if (g_is_w_pressed)
	{
		pong->_paddleLeftSpeed = 0.001f;
	}
	else if (g_is_s_pressed)
	{
		pong->_paddleLeftSpeed = -0.001f;
	}
	else
	{
		pong->_paddleLeftSpeed = 0.0f;
	}

	pong->moveLeftPaddle(pong->_paddleLeftSpeed);
}

void updateRightPaddle(mjf::Pong* pong)
{
	if (g_is_i_pressed)
	{
		pong->_paddleRightSpeed = 0.001f;
	}
	else if (g_is_k_pressed)
	{
		pong->_paddleRightSpeed = -0.001f;
	}
	else
	{
		pong->_paddleRightSpeed = 0.0f;
	}

	pong->moveRightPaddle(pong->_paddleRightSpeed);
}


void keyboardHandler(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 'w':
	case 'W': g_is_w_pressed = !g_is_w_pressed;
		break;
	case 's':
	case 'S':
		g_is_s_pressed = !g_is_s_pressed;
		break;
	case 'i':
	case 'I':
		g_is_i_pressed = !g_is_i_pressed;
		break;
	case 'k':
	case 'K':
		g_is_k_pressed = !g_is_k_pressed;
		break;
	case 'a':
	case 'A':
		g_ball_xSpeed *= 1.1f;
		g_ball_ySpeed *= 1.1f;
		break;
	case 'z':
	case 'Z':
		g_ball_xSpeed /= 1.1f;
		g_ball_ySpeed /= 1.1f;
		break;
	}

}

void keyboardUpHandler(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 'w': 
	case 'W': 
		g_is_w_pressed = false;
		break;
	case 's':
	case 'S':
		g_is_s_pressed = false;
		break;
	case 'i':
	case 'I':
		g_is_i_pressed = false;
		break;
	case 'k':
	case 'K':
		g_is_k_pressed = false;
		break;
	}
}

void updateBall(mjf::Pong* pong)
{
	pong->moveBall(g_ball_xSpeed, g_ball_ySpeed);
	
	// Bounce the ball when collision with the left paddle
	if (pong->ballHasCollisionWithLeftPaddle())
	{
		if (g_ball_xSpeed < 0) g_ball_xSpeed *= -1;
		g_ball_ySpeed = pong->_paddleLeftSpeed;
	}

	// Bounce the ball when collision with the right paddle
	if (pong->ballHasCollisionWithRightPaddle())
	{
		if (g_ball_xSpeed > 0) g_ball_xSpeed *= -1;
		g_ball_ySpeed = pong->_paddleRightSpeed;
	}

	// Bounce the ball when collision with the top or bottom of the screen
	if (pong->getBall().getOrigin().y > 0.8f || pong->getBall().getOrigin().y < -0.8f)
	{
		g_ball_ySpeed = -g_ball_ySpeed;
	}
}

