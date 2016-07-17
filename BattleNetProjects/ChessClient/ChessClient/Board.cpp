#include "stdafx.h"
#include "Board.h"

Board::Board()
{
}
Board::Board(int startX, int startY, int width)
{
	_startX = startX;
	_startY = startY;
	_width = width;
}
void Board::initBoard(PlayerType::Player player, PlayerType::Player opponent)
{

	bool isBlack = true;
	for (int i = 0; i < 8; i++)
	{
		isBlack = !isBlack;
		for (int j = 0; j < 8; j++)
		{
			board[i][j] = Tile();
			board[i][j]._x = j;
			board[i][j]._y = i;
			board[i][j]._height = _width;
			board[i][j]._width = _width;
			board[i][j]._posH = i*_width + _startY;
			board[i][j]._posW = j*_width + _startX;
			board[i][j]._isBlack = isBlack;
			if(i>=6)
			{
				board[i][j]._player= player;
			}
			if(i<=1)
			{
				board[i][j]._player= opponent;
			}
			if(i == 6 || i == 1)
			{
				board[i][j]._piece = PieceType::PAWN;
			}
			if(i == 7 || i == 0)
			{
				if(j == 0 || j == 7)
				{
					board[i][j]._piece = PieceType::ROOK;
				}
				if(j == 1 || j == 6)
				{
					board[i][j]._piece = PieceType::KNIGHT;
				}
				if(j == 2 || j == 5)
				{
					board[i][j]._piece = PieceType::BISHOP;
				}
				if(j == 3)
				{
					board[i][j]._piece = PieceType::QUEEN;
				}
				if(j == 4)
				{
					board[i][j]._piece = PieceType::KING;
				}
			}
			isBlack = !isBlack;
		}
	}
}

Tile* Board::findTile(int x, int y)
{
	int row = -1;
	int col = -1;
	for(int i = 0; i < 8; i++)
	{
		if(board[i][0]._posH <= y && board[i][0]._posH + board[i][0]._height >= y)
		{
			row = i;
		}
		if(board[0][i]._posW <= x && board[0][i]._posW + board[0][i]._width >= x)
		{
			col = i;
		}
		if(row != -1 && col != -1)
		{
			return &board[row][col];
		}
	}
	return &board[0][0];
}