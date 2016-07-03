#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include "Board.h"

namespace GUI
{
	void initialize(int argc, char* argv[]);
	void initTextures();
	void prepareTextures(char* filename, int textureNum);
	void update();
	void special(int key, int, int);
	void render();
	void renderPieces(int i, int j);
	void changeViewport(int x, int h);
	struct GUIData
	{
		GUIData()
			: _boardInit(false)
		{
		};
		void initBoard(int w, int h);
		void setBoardSize(int w, int h);
		int getSize(int w, int h);
		int getStartWidth(int w, int size);
		int getStartHeight(int h, int size);
		void displayText();
		Board _board;
		int windowWidth;
		int windowHeight;
		bool _boardInit;
	};
	const GUIData& GetData();
	GUIData& MutableData();
}