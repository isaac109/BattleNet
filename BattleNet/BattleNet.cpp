// BattleNet.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GUI.h"
#include <iostream>
using namespace std;


int _tmain(int argc, char* argv[])
{
	GUI::initialize(argc, argv);
	while(true)
	{
		GUI::update();
	}
	int hold;
	cin >> hold;
	return 0;
}

