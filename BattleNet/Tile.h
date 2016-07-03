#pragma once

namespace PlayerType
{
	enum Player {P_ONE, P_TWO, NONE };
}
namespace PieceType
{
	enum Piece {KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, NONE };
}

class Tile
{
	
public:
	Tile();
	Tile(int height, int width, int posH, int posW, bool isBlack, PlayerType::Player player, PieceType::Piece piece);
	int _height;
	int _width;
	int _posH;
	int _posW;
	bool _isBlack;
	PlayerType::Player _player;
	PieceType::Piece _piece;
	
	
};

