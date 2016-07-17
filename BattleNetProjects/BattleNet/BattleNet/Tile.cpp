#include "stdafx.h"
#include "Tile.h"

Tile::Tile()
{
	_player = PlayerType::NONE;
	_piece = PieceType::NONE;
	_isSelected = false;
	_isHighlighted = false;
}

Tile::Tile(int height, int width, int posH, int posW, bool isBlack, PlayerType::Player player, PieceType::Piece piece)
{
	_height = height;
	_width = width;
	_posH = posH;
	_posW = posW;
	_isBlack = isBlack;
	_player = player;
	_piece = piece;
	_isSelected = false;
	_isHighlighted = false;
}