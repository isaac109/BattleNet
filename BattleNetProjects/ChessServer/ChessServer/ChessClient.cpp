#include "stdafx.h"
#include "ChessClient.h"

ChessClient::ChessClient(int _id, SOCKET conn)
{
	id = _id;
	connection = conn;
	gameState = GameState::G_GlobalServer;
	chatState = ChatState::C_World;
	opponentID = -1;
}