// BattleNet.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GUI.h"
#include "Client.h"
using namespace std;


//SOCKET Connection;
//int _argc;
//char** _argv;
//
//void runChess(int index)
//{
//	PlayerType::Player player = PlayerType::NONE;
//	if(index == 1)
//	{
//		player = PlayerType::P_ONE;
//	}
//	else
//	{
//		player = PlayerType::P_TWO;
//	}
//	GUI::initialize(_argc, _argv, player);
//	while(true)
//	{
//		GUI::update();
//	}
//}
//
//void ClientThread()
//{
//	int bufferLen;
//	while(true)
//	{
//		recv(Connection, (char*)&bufferLen, sizeof(int), NULL);
//		char* buffer = new char[bufferLen+1];
//		buffer[bufferLen] = '\0';
//		recv(Connection, buffer, bufferLen, NULL);
//
//		string initString1 = "InitPlayer1";
//		string initString2 = "InitPlayer2";
//		if(strcmp(initString1.c_str(),buffer) == 0)
//		{
//			cout<<"player1"<<endl;
//			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)runChess, (LPVOID)(1), NULL, NULL);
//		}
//		else if(strcmp(initString2.c_str(),buffer) == 0)
//		{
//			cout<<"player2"<<endl;
//			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)runChess, (LPVOID)(2), NULL, NULL);
//		}
//		else
//		{
//			cout<<buffer<<endl;
//		}
//
//		delete[] buffer;
//	}
//}

int _argc;
char** _argv;

void runChess(Client& client)
{
	while(true)
	{
		if(client.gState!= GameState::G_PublicChat)
		{
			PlayerType::Player player;
			if(client.gState == GameState::G_InChessP1)
			{
				player = PlayerType::P_ONE;
			}
			if(client.gState == GameState::G_InChessP2)
			{
				player = PlayerType::P_TWO;
			}
			GUI::initialize(_argc, _argv, player);
			while(true)
			{
				GUI::update();
			}
		}
	}
}

int _tmain(int argc, char* argv[])
{
	_argc = argc;
	_argv = argv;
	Client myClient("127.0.0.1",1111); //Create client to localhost ("127.0.0.1") on port 1111

	if (!myClient.Connect()) //If client fails to connect...
	{
		std::cout << "Failed to connect to server..." << std::endl;
		system("pause");
		return 1;
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)runChess, (LPVOID)(&myClient), NULL, NULL);
	std::string userinput; //holds the user's chat message
	while (true)
	{
		std::getline(std::cin,userinput); //Get line if user presses enter and fill the buffer
		if (!myClient.SendString(userinput)) //Send string: userinput, If string fails to send... (Connection issue)
			break; //If send string failed (if connection was closed), leave this loop since we have lost connection to the server
	}

	system("pause");
	return 0;
}

