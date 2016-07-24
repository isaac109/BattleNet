#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib") //Required for WinSock
#include <WinSock2.h> //For win sockets
#include <string> //For std::string
#include <iostream> //For std::cout, std::endl, std::cin.getline
#include "GUI.h"

enum Packet
{
	P_ChatMessage,
	P_GameStateChange,
	P_ChessMove,
	P_Test
};

enum GameState
{
	G_PublicChat,
	G_InChessP1,
	G_InChessP2
};

class Client
{
public: //Public functions
	Client(std::string IP, int PORT);
	bool Connect();

	bool SendString(std::string & _string);
	bool CloseConnection();

	
	GameState gState;
	int _argc;
	char** _argv;
private: //Private functions
	bool ProcessPacket(Packet _packettype);
	static void ClientThread();
	static void PlayChess(PlayerType::Player player);
	//Sending Funcs
	bool SendInt(int _int);
	bool SendPacketType(Packet _packettype);
	

	//Getting Funcs
	bool GetInt(int & _int);
	bool GetPacketType(Packet & _packettype);
	bool GetString(std::string & _string);

private:
	SOCKET Connection;//This client's connection to the server
	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	int sizeofaddr; //Need sizeofaddr for the connect function


	
};

static Client * clientptr; //This client ptr is necessary so that the ClientThread method can access the Client instance/methods. Since the ClientThread method is static, this is the simplest workaround I could think of since there will only be one instance of the client.