#pragma once
#include <GameEngine/SpriteBatch.h>
#include "glm.hpp"

struct Cell
{
	bool isWall;
	float pressure;
	int x;
	int y;
};

class Grid
{
public:
	Grid(GameEngine::SpriteBatch* _sb);
	~Grid();
	void draw(int _x, int _y);
	void runGasSimulation();
	void addPressure(int _x, int _y, float _amount);
	void reducePressure(int _x, int _y, float _amount);
	void setAsWall(int _x, int _y, bool isWall);
	void reset();
	void addWall(int _x, int _y);
	void removeWall(int _x, int _y);
private:
	int cellSize = 10;
	const int gridSize = 50;
	Cell grid[50][50];
	GameEngine::SpriteBatch* sb;
};