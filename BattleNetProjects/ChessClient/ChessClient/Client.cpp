#include "stdafx.h"
#include "Client.h"
#include <string>

bool Client::ProcessPacket(Packet _packettype)
{
	switch (_packettype)
	{
	case P_ChatMessage: //If packet is a chat message packet
	{
		std::string Message; //string to store our message we received
		if (!GetString(Message)) //Get the chat message and store it in variable: Message
			return false; //If we do not properly get the chat message, return false
		std::cout << Message << std::endl; //Display the message to the user
		break;
	}
	case P_GameStateChange:
		{
			std::string Message; //string to store our message we received
			if (!GetString(Message)) //Get the chat message and store it in variable: Message
				return false; //If we do not properly get the chat message, return false
			std::cout << Message << std::endl; //Display the message to the user
			if(Message.find("InitChess") != std::string::npos)
			{
				opponentID = Message.substr(Message.find(":")+1);
				if(Message.find("P1") != std::string::npos)
				{
					clientptr->gState = GameState::G_InChessP1;
					std::cout<<"Starting Chess as Player 1"<<std::endl;
					CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PlayChess, (LPVOID)(PlayerType::P_ONE), NULL, NULL);
				}
				else if(Message.find("P2") != std::string::npos)
				{
					clientptr->gState = GameState::G_InChessP2;
					std::cout<<"Starting Chess as Player 2"<<std::endl;
					CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PlayChess, (LPVOID)(PlayerType::P_TWO), NULL, NULL);
				}
			}
			break;
		}
	case P_ChessMove:
		{
			std::string Message; //string to store our message we received
			if (!GetString(Message)) //Get the chat message and store it in variable: Message
				return false; //If we do not properly get the chat message, return false
			std::cout << Message << std::endl;
			GUI::moveOpponent(Message);
			break;
		}
	default: //If packet type is not accounted for
		std::cout << "Unrecognized packet: " << _packettype << std::endl; //Display that packet was not found
		break;
	}
	return true;
}

void Client::ClientThread()
{
	Packet PacketType;
	while (true)
	{
		if (!clientptr->GetPacketType(PacketType)) //Get packet type
			break; //If there is an issue getting the packet type, exit this loop
		if (!clientptr->ProcessPacket(PacketType)) //Process packet (packet type)
			break; //If there is an issue processing the packet, exit this loop
	}
	std::cout << "Lost connection to the server." << std::endl;
	if (clientptr->CloseConnection()) //Try to close socket connection..., If connection socket was closed properly
	{
		std::cout << "Socket to the server was closed successfuly." << std::endl;
	}
	else //If connection socket was not closed properly for some reason from our function
	{
		std::cout << "Socket was not able to be closed." << std::endl;
	}
}

Client::Client(std::string IP, int PORT)
{
	sizeofaddr = sizeof(addr);

	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	addr.sin_addr.s_addr = inet_addr(IP.c_str()); //Address 127.0.0.1 = localhost (This pc)
	addr.sin_port = htons(PORT); //Port to connect on
	addr.sin_family = AF_INET; //IPv4 Socket
	gState = GameState::G_PublicChat;
	clientptr = this; //Update ptr to the client which will be used by our client thread
}

bool Client::Connect()
{
	Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) //If we are unable to connect...
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	std::cout << "Connected!" << std::endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Create the client thread that will receive any data that the server sends.
	return true;
}

bool Client::CloseConnection()
{
	if (closesocket(Connection) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) //If socket error is that operation is not performed on a socket (This happens when the socket has already been closed)
			return true; //return true since connection has already been closed

		std::string ErrorMessage = "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError()) + ".";
		MessageBoxA(NULL, ErrorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

void Client::PlayChess(PlayerType::Player player)
{
	GUI::initialize(clientptr->_argc, clientptr->_argv, player);
	while(true)
	{
		std::string msg = GUI::update();
		if(msg.find("MOVE:") != std::string::npos)
		{
			msg+="OpponentID:" + clientptr->opponentID;
			if (!clientptr->SendString(msg)) //Send string: userinput, If string fails to send... (Connection issue)
			break;
		}
	}
}