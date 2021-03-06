#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include <vector>

#include <GameEngine/GameEngine.h>
#include <GameEngine/GLSLProgram.h>
#include <GameEngine/GLTexture.h>
#include <GameEngine/Sprite.h>
#include <GameEngine/Window.h>
#include <GameEngine/InputManager.h>
#include <GameEngine/Timing.h>
#include <GameEngine/CollisionDetection.h>
#include <GameEngine/SpriteBatch.h>
#include <GameEngine/Camera2D.h>
#include <GameEngine/DrawText.h>
#include <GameEngine/Sound.h>

#include "Grid.h"


enum class GameState { PLAY, EXIT };

class Program
{
public:
	Program();
	~Program();

	void run();
private:

	GameEngine::CollisionDetection collisionDetection;
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();
	void updateGame();
	GameEngine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	GameEngine::GLSLProgram _colorProgram;
	GameEngine::Camera2D _camera;

	GameEngine::SpriteBatch spriteBatch;

	GameEngine::DrawText drawText;

	GameEngine::InputManager _inputManager;
	GameEngine::FpsLimiter _fpsLimiter;

	GameEngine::Sound sound;

	float _maxFPS;
	float _fps;
	float _time;
	bool lastPressedL = false;
	bool lastPressedR = false;

	Grid* grid;
};
