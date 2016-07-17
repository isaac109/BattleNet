#pragma once
#include <WinSock2.h>

class ChessPair
{
public:
	SOCKET _player1;
	SOCKET _player2;
	ChessPair();
	ChessPair(SOCKET p1, SOCKET p2);
	void InitGame();
};