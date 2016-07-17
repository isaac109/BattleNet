// ChessServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Server MyServer(1111); //Create server on port 100
	for (int i = 0; i < 100; i++) //Up to 100 times...
	{
		MyServer.ListenForNewConnection(); //Accept new connection (if someones trying to connect)
	}
	system("pause");
	return 0;
	return 0;
}

