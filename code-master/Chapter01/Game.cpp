// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mPaddleDir(0)
,mPaddleDir2(0)
{
	
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	//第一个拍的位置
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f/2.0f;

	//第二个拍的位置
	mPaddlePos2.x = 1024.0f - 20.0f;
	mPaddlePos2.y = 768.0f / 2.0f;

	//第一个球
	mBallPos.x = 1024.0f/2.0f;
	mBallPos.y = 768.0f/2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

	//第二个球
	mBallPos2.x = 1024.0f / 2.0f;
	mBallPos2.y = 768.0f / 2.0f;
	mBallVel2.x = 200.0f;
	mBallVel2.y = -235.0f;


	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	
	// Update paddle direction based on W/S keys
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}

	//更新拍的方向
	mPaddleDir2 = 0;
	if (state[SDL_SCANCODE_I])
	{
		mPaddleDir2 -= 1;
	}
	if (state[SDL_SCANCODE_K])
	{
		mPaddleDir2 += 1;
	}



}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	
	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();
	
	// Update paddle position based on direction
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddlePos.y < (paddleH/2.0f + thickness))
		{
			mPaddlePos.y = paddleH/2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - paddleH/2.0f - thickness))
		{
			mPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
		}
	}
	//更新拍的移动
	if (mPaddleDir2 != 0)
	{
		mPaddlePos2.y += mPaddleDir2 * 300.0f * deltaTime;

		if (mPaddlePos2.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos2.y  = paddleH / 2.0F + thickness;
		}
		else if (mPaddlePos2.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos2.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}





	// Update ball position based on ball velocity
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	//第二个球位置更新
	mBallPos2.x += mBallVel2.x * deltaTime;
	mBallPos2.y += mBallVel2.y * deltaTime;
	
	// Bounce if needed
	// Did we intersect with the paddle?
	float diff = mPaddlePos.y - mBallPos.y;
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;

	// 第二个球拍检测
	float diff2 = mPaddlePos2.y - mBallPos.y;
	diff2 = (diff2 > 0.0f) ? diff2 : -diff2;



	if ( ((diff <= paddleH / 2.0f &&mBallPos.x <= 25.0f && mBallPos.x >= 20.0f && mBallVel.x < 0.0f  )   ||   ( diff2 <= paddleH/2.0f && mBallPos.x >= 996.5f && mBallPos.x <=1000.0f  && mBallVel.x >0.0f ))
		|| ((diff <= paddleH / 2.0f && mBallPos2.x <= 25.0f && mBallPos2.x >= 20.0f && mBallVel2.x < 0.0f) || (diff2 <= paddleH / 2.0f && mBallPos2.x >= 996.5f && mBallPos2.x <= 1000.0f && mBallVel2.x > 0.0f)))
	{
		mBallVel.x *= -1.0f;
		mBallVel2.x *= -1.0f;
	}
	// Did the ball go off the screen? (if so, end game)
	else if ((mBallPos.x <= 0.0f || mBallPos.x>1024.0f) || (mBallPos2.x <= 0.0f || mBallPos2.x > 1024.0f))
	{
		mIsRunning = false;
	}
	// Did the ball collide with the right wall?    右边墙体碰撞
   //	else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
	//{
	//	mBallVel.x *= -1.0f;
	//}
	
	// Did the ball collide with the top wall?
	if ((mBallPos.y <= thickness && mBallVel.y < 0.0f)||(mBallPos2.y<=thickness && mBallVel2.y<0.0f))
	{
		mBallVel.y *= -1;
		mBallVel2.y *= -1;
	}
	// Did the ball collide with the bottom wall?
	else if ((mBallPos.y >= (768 - thickness) &&mBallVel.y > 0.0f)|| (mBallPos2.y >= (768 - thickness) && mBallVel2.y > 0.0f))
	{
		mBallVel.y *= -1;
		mBallVel2.y *= -1;
	}
}

void Game::GenerateOutput()
{
	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		0,	    // B
		255		// A
	);
	
	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 0, 255, 255);
	
	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	//   右边墙体
	// Draw right wall
	//wall.x = 1024 - thickness;
	//wall.y = 0;
	//wall.w = thickness;
	//wall.h = 1024;
	//SDL_RenderFillRect(mRenderer, &wall);

	
	// Draw paddle
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	//画第二个拍
	SDL_Rect paddle2{
		static_cast<int>(mPaddlePos2.x),
		static_cast<int>(mPaddlePos2.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle2);
	
	// Draw ball
	SDL_Rect ball{	
		static_cast<int>(mBallPos.x - thickness/2),
		static_cast<int>(mBallPos.y - thickness/2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_Rect ball2{
		static_cast<int>(mBallPos2.x - thickness / 2),
		static_cast<int>(mBallPos2.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball2);


	
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);//关闭渲染器
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
