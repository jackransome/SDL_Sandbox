#include "Grid.h"
#include <GameEngine/GameEngine.h>
#include <iostream>
#include <functional>
#include <algorithm>
struct nextResult { bool left; bool right; bool up; bool down; };
struct nextResultValues { float left; float right; float up; float down; };
Grid::Grid(GameEngine::SpriteBatch* _sb){
	sb = _sb;
	reset();
}

Grid::~Grid() {}

void Grid::draw(int _x, int _y){
	_x -= (gridSize + gridSize * 10) / 2;
	_y -= (gridSize + gridSize * 10) / 2;
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			GameEngine::Color color;
			if (grid[i][j][currentState].isWall)
			{
				color.r = 200;
				color.g = 200;
				color.b = 200;
				color.a = 255;
			}
			else
			{
				color.r = 0;
				color.g = 0;
				color.b = 0;
				color.a = 0;
				color.b += (255 - color.b)*(1 - float(grid[i][j][currentState].pressure) / 10);
				color.r += (255 - color.r)*(float(grid[i][j][currentState].pressure) / 10);
				if (grid[i][j][currentState].pressure > 10)
				{
					color.r = 255;
					color.b = 0;
				}
			}
			GameEngine::drawRect(_x + i * 10 + i, _y + j * 10 + j, cellSize, cellSize, 1, color, sb);
		}
	}
}
void Grid::reset(){
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			grid[i][j][currentState] = Cell();
			grid[i][j][currentState].x = i;
			grid[i][j][currentState].y = j;
			grid[i][j][currentState].isWall = false;
			grid[i][j][currentState].pressure = 0;
			grid[i][j][nextState] = Cell();
			grid[i][j][nextState].x = i;
			grid[i][j][nextState].y = j;
			grid[i][j][nextState].isWall = false;
			grid[i][j][nextState].pressure = 0;
		}
	}
}
void Grid::addWall(int _x, int _y){
	if (_x < 0 || _y < 0 || _x >= gridSize || _y >= gridSize) {
		return;
	}
	grid[_x][_y][currentState].isWall = true;
	grid[_x][_y][nextState].isWall = grid[_x][_y][currentState].isWall;
}
void Grid::removeWall(int _x, int _y){
	if (_x < 0 || _y < 0 || _x >= gridSize || _y >= gridSize) {
		return;
	}
	grid[_x][_y][currentState].isWall = false;
	grid[_x][_y][nextState].isWall = grid[_x][_y][currentState].isWall;
}
bool cmd(const Cell* c1, const Cell* c2){
	return c1->pressure < c2->pressure;
}
void Grid::runGasSimulation(){
	//reseting next state:
	for (int x = 0; x < gridSize; x++) {
		for (int y = 0; y < gridSize; y++) {
			grid[x][y][nextState].pressure = 0;
		}
	}
	for (int x = 0; x < gridSize; x++)
	{
		for (int y = 0; y < gridSize; y++)
		{
			nextResult result = { false, false, false, false };
			nextResultValues resultValues = { 0,0,0,0 };
			int resultInt = 0;
			float highestDifference = 0;
			//checking if neighbouring cell has less pressure than the current cell
			//left
			if (x > 0 && !grid[x - 1][y][currentState].isWall && grid[x - 1][y][currentState].pressure < grid[x][y][currentState].pressure)
			{
				resultInt++;
				result.left = true;
				resultValues.left = grid[x][y][currentState].pressure - grid[x - 1][y][currentState].pressure;
			}
			//right
			if (x < gridSize - 1 && !grid[x + 1][y][currentState].isWall && grid[x + 1][y][currentState].pressure < grid[x][y][currentState].pressure)
			{
				resultInt++;
				result.right = true;
				resultValues.right = grid[x][y][currentState].pressure - grid[x + 1][y][currentState].pressure;
			}
			//up
			if (y < gridSize - 1 && !grid[x][y + 1][currentState].isWall && grid[x][y + 1][currentState].pressure < grid[x][y][currentState].pressure)
			{
				resultInt++;
				result.up = true;
				resultValues.up = grid[x][y][currentState].pressure - grid[x][y + 1][currentState].pressure;
			}
			//down
			if (y > 0 && !grid[x][y - 1][currentState].isWall && grid[x][y - 1][currentState].pressure < grid[x][y][currentState].pressure)
			{
				resultInt++;
				result.down = true;
				resultValues.down = grid[x][y][currentState].pressure - grid[x][y - 1][currentState].pressure;
			}
			float amount = resultValues.up + resultValues.down + resultValues.left + resultValues.right;
			float amount2 = grid[x][y][currentState].pressure / 2;
			int resultTotal = result.down + result.left + result.up + result.right;

			//adding the changes
			/*if (result.left)
			{
				grid[x][y][nextState].pressure += - amount2 * resultValues.left / amount;
				grid[x - 1][y][nextState].pressure += amount2 * resultValues.left / amount;
			}
			if (result.right)
			{
				grid[x][y][nextState].pressure += - amount2 * resultValues.right / amount;
				grid[x + 1][y][nextState].pressure += amount2 * resultValues.right / amount;
			}
			if (result.up)
			{
				grid[x][y][nextState].pressure += - amount2 * resultValues.up / amount;
				grid[x][y + 1][nextState].pressure += amount2 * resultValues.up / amount;
			}
			if (result.down)
			{
				grid[x][y][nextState].pressure += - amount2 * resultValues.down / amount;
				grid[x][y - 1][nextState].pressure += amount2 * resultValues.down / amount;
			}*/
			newSimulation(x, y);
		}
	}
	//adding previous state(current) to the changes(next)
	for (int x = 0; x < gridSize; x++) {
		for (int y = 0; y < gridSize; y++) {
			grid[x][y][nextState].pressure += grid[x][y][currentState].pressure;
		}
	}
	if (currentState) {
		currentState--;
		nextState++;
	}
	else {
		currentState++;
		nextState--;
	}
}

void Grid::addPressure(int _x, int _y, float _amount){
	if (_x < 0 || _y < 0 || _x >= gridSize || _y >= gridSize) {
		return;
	}
	grid[_x][_y][currentState].pressure += _amount;
	grid[_x][_y][nextState] = grid[_x][_y][currentState];
}
void Grid::reducePressure(int _x, int _y, float _amount) {
	if (_x < 0 || _y < 0 || _x >= gridSize || _y >= gridSize) {
		return;
	}
	if (grid[_x][_y][currentState].pressure > _amount)
	{
		grid[_x][_y][currentState].pressure -= _amount;
	}
	else
	{
		grid[_x][_y][currentState].pressure = 0;
	}
	grid[_x][_y][nextState] = grid[_x][_y][currentState];
}

void Grid::newSimulation(int _x, int _y){
	if (!grid[_x][_y][currentState].pressure) {
		return;
	}

	if (_x > 0 && !grid[_x - 1][_y][nextState].isWall) {
		grid[_x][_y][nextState].pressure += -grid[_x][_y][currentState].pressure / 5;
		grid[_x - 1][_y][nextState].pressure += grid[_x][_y][currentState].pressure / 5;
	}

	if (_x < gridSize - 1 && !grid[_x + 1][_y][nextState].isWall) {
		grid[_x][_y][nextState].pressure += -grid[_x][_y][currentState].pressure / 5;
		grid[_x + 1][_y][nextState].pressure += grid[_x][_y][currentState].pressure / 5;
	}

	if (_y < gridSize - 1 && !grid[_x][_y + 1][nextState].isWall) {
		grid[_x][_y][nextState].pressure += -grid[_x][_y][currentState].pressure / 5;
		grid[_x][_y + 1][nextState].pressure += grid[_x][_y][currentState].pressure / 5;
	}

	if (_y > 0 && !grid[_x][_y - 1][nextState].isWall) {
		grid[_x][_y][nextState].pressure += -grid[_x][_y][currentState].pressure / 5;
		grid[_x][_y - 1][nextState].pressure += grid[_x][_y][currentState].pressure / 5;
	}
}
