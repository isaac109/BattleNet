// BattleNetServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"
using namespace std;

void OrganizeGames()
{
	/*while(true)
	{
		if(Connections.size() > 1)
		{
			ChessPair pair = ChessPair(Connections[0], Connections[1]);
			pair.InitGame();
			chessPairs.push_back(pair);
			string buffer = "Sendexit";
			int bufferLen = buffer.size();
			Connections.erase(Connections.begin());
			Connections.erase(Connections.begin());
		}
	}*/
}

//void ClientHandlerThread(int index)
//{
//	int bufferLen;
//	while(true)
//	{
//		recv(Connections[index], (char*)&bufferLen, sizeof(int), NULL);
//		char* buffer = new char[bufferLen+1];
//		recv(Connections[index], buffer, bufferLen, NULL);
//		cout<<"here"<<endl;
//		buffer[bufferLen] = '\0';
//		string exitString = "exit";
//		if(strcmp(exitString.c_str(),buffer) == 0)
//		{
//			Connections.erase(Connections.begin() + index);
//			return;
//		}
//		for(int i = 0; i < Connections.size(); i++)
//		{
//			if(i == index)
//				continue;
//			send(Connections[i], (char*)&bufferLen, sizeof(int), NULL);
//			send(Connections[i], buffer, bufferLen, NULL);
//		}
//
//		delete[] buffer;
//	}
//}

int _tmain(int argc, _TCHAR* argv[])
{
	Server MyServer(1111); //Create server on port 100
	for (int i = 0; i < 100; i++) //Up to 100 times...
	{
		MyServer.ListenForNewConnection(); //Accept new connection (if someones trying to connect)
	}
	system("pause");
	return 0;
}

