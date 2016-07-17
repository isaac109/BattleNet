#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include "Board.h"
#include <list>

namespace GUI
{
	void initialize(int argc, char* argv[], PlayerType::Player player);
	void initTextures();
	void prepareTextures(char* filename, int textureNum);
	void update();
	void special(int key, int, int);
	void mouseClick(int button, int state, int x, int y);
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
		void highlightOptions(int w, int h, bool clear);
		void highlighBishop(int w, int h);
		void highlighKing(int w, int h);
		void highlighKnight(int w, int h);
		void highlighPawn(int w, int h);
		void highlighQueen(int w, int h);
		void highlighRook(int w, int h);
		void displayText();
		Board _board;
		int windowWidth;
		int windowHeight;
		bool _boardInit;
		PlayerType::Player _player;
		PlayerType::Player _opponent;
		Tile* selectedTile;
		std::list<Tile*> highlightTiles;

	};
	const GUIData& GetData();
	GUIData& MutableData();
}