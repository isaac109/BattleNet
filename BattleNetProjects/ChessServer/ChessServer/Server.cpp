#include "stdafx.h"
#include "Server.h"
#include <algorithm>


Server::Server(int PORT, bool BroadcastPublically) //Port = port to broadcast on. BroadcastPublically = false if server is not open to the public (people outside of your router), true = server is open to everyone (assumes that the port is properly forwarded on router settings)
{
	//Winsock Startup
	TotalConnections = 0;
	addrlen = sizeof(addr);

	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (BroadcastPublically) //If server is open to public
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else //If server is only for our router
		addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Broadcast locally
	addr.sin_port = htons(PORT); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listen for new connections
	if (bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) //Bind the address to the socket, if we fail to bind the address..
	{
		std::string ErrorMsg = "Failed to bind the address to our listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR) //Places sListen socket in a state in which it is listening for an incoming connection. Note:SOMAXCONN = Socket Oustanding Max Connections, if we fail to listen on listening socket...
	{
		std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	serverptr = this;

	CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)StartChessGames, NULL, NULL, NULL);

}

bool Server::ListenForNewConnection()
{
	SOCKET newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection
	if (newConnection == 0) //If accepting the client connection failed
	{
		std::cout << "Failed to accept the client's connection." << std::endl;
		return false;
	}
	else //If client connection properly accepted
	{
		std::cout << "Client Connected! ID:" << TotalConnections << std::endl;
		ChessClient player = ChessClient(TotalConnections,newConnection);
		players.push_back(player);
		threads.push_back(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(TotalConnections), NULL, NULL));
		std::string MOTD = "MOTD: Welcome! This is the message of the day!.";
		SendString(TotalConnections, MOTD);
		TotalConnections += 1; //Incremenent total # of clients that have connected
		return true;
	}
}

bool Server::ProcessPacket(int IDS, int IDR, Packet _packettype)
{
	switch (_packettype)
	{
	case P_ChatMessage: //Packet Type: chat message
	{
		std::string Message; //string to store our message we received
		if (!GetString(IDS, Message)) //Get the chat message and store it in variable: Message
			return false; //If we do not properly get the chat message, return false
						  //Next we need to send the message out to each user
		if (!SendString(IDR, Message)) //Send message to connection at index i, if message fails to be sent...
		{
			std::cout << "Failed to send message from client ID: " << IDS << " to client ID: " << IDR << std::endl;
		}

		std::cout << "Processed chat message packet from user ID: " << IDS << std::endl;
		break;
	}
	default: //If packet type is not accounted for
	{
		std::cout << "Unrecognized packet: " << _packettype << std::endl; //Display that packet was not found
		break;
	}
	}
	return true;
}

bool Server::ProcessPacket(int ID, Packet _packettype)
{
	int _id = FindClient(ID);
	if(_id == -1)
	{
		return false;
	}
	ChessClient& client = players[_id];

	switch (_packettype)
	{
	case P_ChatMessage: //Packet Type: chat message
		{
			std::string Message; //string to store our message we received
			if (!GetString(ID, Message)) //Get the chat message and store it in variable: Message
				return false; //If we do not properly get the chat message, return false
							  //Next we need to send the message out to each user
			if(client.chatState == ChatState::C_World)
			{
				for (int i = 0; i < players.size(); i++)
				{
					if (players[i].id == client.id) //If connection is the user who sent the message...
						continue;//Skip to the next user since there is no purpose in sending the message back to the user who sent it.
					if (!SendString(players[i].id, Message)) //Send message to connection at index i, if message fails to be sent...
					{
						std::cout << "Failed to send message from client ID: " << ID << " to client ID: " << players[i].id << std::endl;
					}
				}
			}
			else if(client.chatState == ChatState::C_InGame)
			{
				if (!SendString(client.opponentID, Message)) //Send message to connection at index i, if message fails to be sent...
				{
					std::cout << "Failed to send message from client ID: " << ID << " to client ID: " << client.opponentID << std::endl;
				}
			}
			std::cout << "Processed chat message packet from user ID: " << ID << std::endl;
			break;
		}
	case P_GameStateChange:
		{
			std::string Message;
			if (!GetString(ID, Message)) //Get the chat message and store it in variable: Message
				return false;
			if(Message == "RequestChess")
			{
				client.gameState = GameState::G_Waiting_For_chess;
				std::cout<< ID<<players[FindClient(ID)].gameState<<std::endl;
			}
			break;
		}
	case P_ChessMove: //Packet Type: chat message
		{
			std::string Message; //string to store our message we received
			if (!GetString(ID, Message)) //Get the chat message and store it in variable: Message
				return false; //If we do not properly get the chat message, return false
							  //Next we need to send the message out to each user
			if (!SendString(client.opponentID, Message)) //Send message to connection at index i, if message fails to be sent...
			{
				std::cout << "Failed to send message from client ID: " << ID << " to client ID: " << client.opponentID << std::endl;
			}
			std::cout << "Processed chat message packet from user ID: " << ID << std::endl;
			break;
		}
	case P_Disconnect:
		{
			std::string Message = "Player " + std::to_string(client.id) + "has disconnected";
			for (int i = 0; i < players.size(); i++)
				{
					if (players[i].id == client.id) //If connection is the user who sent the message...
						continue;//Skip to the next user since there is no purpose in sending the message back to the user who sent it.
					if (!SendString(players[i].id, Message)) //Send message to connection at index i, if message fails to be sent...
					{
						std::cout << "Failed to send message from client ID: " << ID << " to client ID: " << players[i].id << std::endl;
					}
				}
			if(client.opponentID != -1)
			{
				Message = "You have won";
				_id = FindClient(client.opponentID);
				players[_id].opponentID = -1;
				if (!SendString(client.opponentID, Message)) //Send message to connection at index i, if message fails to be sent...
				{
					std::cout << "Failed to send message from client ID: " << ID << " to client ID: " << client.opponentID << std::endl;
				}
			}
			break;
		}
	default: //If packet type is not accounted for
		{
			std::cout << "Unrecognized packet: " << _packettype << std::endl; //Display that packet was not found
			break;
		}
	}
	return true;
}

int Server::FindClient(int ID)
{
	for(int i = 0; i < players.size(); i++)
	{
		if(players[i].id == ID)
		{
			return i;
		}
	}
	return -1;
}

void Server::StartChessGames()
{
	std::vector<int> waiting;
	while(true)
	{
		for (int i = 0; i < serverptr->players.size(); i++)
		{
			if(serverptr->players[i].gameState == GameState::G_Waiting_For_chess && std::find(waiting.begin(), waiting.end(), serverptr->players[i].id) == waiting.end())
			{
				waiting.push_back(serverptr->players[i].id);
			}
			
		}
		for(int i = 0; i < waiting.size(); i++)
		{
			if(serverptr->FindClient(waiting[i]) == -1)
			{
				waiting.erase(waiting.begin()+i);
			}
		}
		if(waiting.size() >= 2)
		{
			std::cout<<"ready"<<std::endl;
			int p1 = waiting[0];
			int p2 = waiting[1];
			waiting.erase(waiting.begin(), waiting.begin()+2);
			serverptr->players[serverptr->FindClient(p1)].gameState = GameState::G_In_Chess;
			serverptr->players[serverptr->FindClient(p2)].gameState = GameState::G_In_Chess;
			serverptr->players[serverptr->FindClient(p1)].opponentID = p2;
			serverptr->players[serverptr->FindClient(p2)].opponentID = p1;
			std::string Message = "InitChessP1";
			if (!serverptr->SendString(p1, Message)) //Send message to connection at index i, if message fails to be sent...
			{
				std::cout << "Failed to send message to client ID: " << p1 << std::endl;
			}
			Message = "InitChessP2";
			if (!serverptr->SendString(p2, Message)) //Send message to connection at index i, if message fails to be sent...
			{
				std::cout << "Failed to send message to client ID: " << p2 << std::endl;
			}

		}
	}
}

void Server::ChessHandlerThread(int IDSender, int IDReciever)
{
	Packet PacketType;
	while(true)
	{
		if (!serverptr->GetPacketType(IDSender, PacketType)) //Get packet type
			break; //If there is an issue getting the packet type, exit this loop
		if (!serverptr->ProcessPacket(IDSender, IDReciever, PacketType)) //Process packet (packet type)
			break; //If there is an issue processing the packet, exit this loop
	}
}

void Server::ClientHandlerThread(int ID) //ID = the index in the SOCKET Connections array
{
	Packet PacketType;
	while (true)
	{
		if (!serverptr->GetPacketType(ID, PacketType)) //Get packet type
			break; //If there is an issue getting the packet type, exit this loop
		if (!serverptr->ProcessPacket(ID, PacketType)) //Process packet (packet type)
			break; //If there is an issue processing the packet, exit this loop
	}
	std::cout << "Lost connection to client ID: " << ID << std::endl;
	serverptr->ProcessPacket(ID, Packet::P_Disconnect);
	closesocket(serverptr->players[serverptr->FindClient(ID)].connection);
	serverptr->players.erase(serverptr->players.begin() + serverptr->FindClient(ID));
}