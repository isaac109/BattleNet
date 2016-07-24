#include <WinSock2.h>

enum ChatState
{
	C_World,
	C_InGame,
	C_None
};

enum GameState
{
	G_GlobalServer,
	G_Waiting_For_chess,
	G_In_Chess,
	G_None
};


class ChessClient
{
public:
	int id;
	int opponentID;
	ChatState chatState;
	GameState gameState;
	SOCKET connection;

	ChessClient(int _id, SOCKET conn);
};