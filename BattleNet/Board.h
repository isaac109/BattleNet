#pragma once
#include "Tile.h"

class Board
{
public:
	Board();
	Board(int startX, int startY, int width);
	void initBoard();
	int _startX;
	int _startY;
	int _width;
	Tile board[8][8];
};