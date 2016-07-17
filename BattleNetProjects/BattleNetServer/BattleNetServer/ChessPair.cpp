#include "stdafx.h"
#include "ChessPair.h"
#include <string>
#include <iostream>
using namespace std;

void GameChannel(SOCKET player, SOCKET opponent);

ChessPair::ChessPair()
{
}

ChessPair::ChessPair(SOCKET p1, SOCKET p2)
{
	_player1 = p1;
	_player2 = p2;
}

void ChessPair::InitGame()
{
	string buffer = "InitPlayer1";
	int bufferLen = buffer.size();
	send(_player1, (char*)&bufferLen, sizeof(int), NULL);
	send(_player1, buffer.c_str(), bufferLen, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)GameChannel, (LPVOID)(_player1, _player2), NULL, NULL);
	
	buffer = "InitPlayer2";
	bufferLen = buffer.size();
	send(_player2, (char*)&bufferLen, sizeof(int), NULL);
	send(_player2, buffer.c_str(), bufferLen, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)GameChannel, (LPVOID)(_player2, _player1), NULL, NULL);
}

void GameChannel(SOCKET player, SOCKET opponent)
{
	int bufferLen;
	while(true)
	{
		recv(player, (char*)&bufferLen, sizeof(int), NULL);
		char* buffer = new char[bufferLen+1];
		recv(player, buffer, bufferLen, NULL);
		cout<<"here1"<<endl;
		buffer[bufferLen] = '\0';
		string exitString = "exit";
		if(strcmp(exitString.c_str(),buffer) == 0)
		{
			return;
		}
		send(opponent, (char*)&bufferLen, sizeof(int), NULL);
		send(opponent, buffer, bufferLen, NULL);

		delete[] buffer;
	}
}