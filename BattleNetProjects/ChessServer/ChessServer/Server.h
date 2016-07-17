#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>

enum Packet
{
	P_ChatMessage,
	P_GameStateChange,
	P_ChessMove,
	P_Test
};

enum GameState
{
	G_GlobalServer,
	G_Waiting_For_chess,
	G_In_Chess,
	G_None
};

class Server
{
public:
	Server(int PORT, bool BroadcastPublically = false);
	bool ListenForNewConnection();

private:
	bool SendInt(int ID, int _int);
	bool GetInt(int ID, int & _int);

	bool SendPacketType(int ID, Packet _packettype);
	bool GetPacketType(int ID, Packet & _packettype);

	bool SendString(int ID, std::string & _string);
	bool GetString(int ID, std::string & _string);

	bool ProcessPacket(int ID, Packet _packettype);
	bool ProcessPacket(int IDS, int IDR, Packet _packettype);

	static void ClientHandlerThread(int ID);
	static void StartChessGames();
	static void ChessHandlerThread(int IDP1, int IDP2);

private:
	SOCKET Connections[100];
	GameState PlayersStates[100];
	bool KillThread[100];
	HANDLE ClientThreads[100];
	HANDLE ChessThreads[50];
	int TotalConnections;

	
	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen;
	SOCKET sListen;
};

static Server * serverptr; //Serverptr is necessary so the static ClientHandler method can access the server instance/functions.
