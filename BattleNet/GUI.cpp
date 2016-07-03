#include "stdafx.h"
#include "GUI.h"
#include <cstring>
#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

GLuint texture[12];

struct Image {

    unsigned long sizeX;
	unsigned long sizeY;
	char *data ;

};

typedef struct Image Image;
#define checkImageWidth 64
#define checkImageHeight 64


GLubyte checkImage[checkImageWidth][checkImageHeight][3];

void makeCheckImage(void){

    int i, j, c;

    for (i = 0; i < checkImageWidth; i++) {

        for (j = 0; j < checkImageHeight; j++) {

            c = ((((i&0x8)==0)^((j&0x8)==0)))*255;

            checkImage[i][j][0] = (GLubyte) c;

            checkImage[i][j][1] = (GLubyte) c;

            checkImage[i][j][2] = (GLubyte) c;

        }

    }

}



int ImageLoad(char *filename, Image *image) {

    FILE *file;
    unsigned long size; // size of the image in bytes.
    unsigned long i; // standard counter.
	unsigned short int planes; // number of planes in image (must be 1)
    unsigned short int bpp; // number of bits per pixel (must be 24)
    char temp; // temporary color storage for bgr-rgb conversion.

    // make sure the file is there.
	errno_t err = fopen_s(&file,filename,"rb");
    if (file==NULL){
        printf("File Not Found : %s\n",filename);
        return 0;
    }

    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);
    // read the width
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
        printf("Error reading width from %s.\n", filename);
        return 0;
    }

    //printf("Width of %s: %lu\n", filename, image->sizeX);
    // read the height

    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
        printf("Error reading height from %s.\n", filename);
        return 0;
    }

    //printf("Height of %s: %lu\n", filename, image->sizeY);
    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;
    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) {
        printf("Error reading planes from %s.\n", filename);
        return 0;
    }

    if (planes != 1) {
        printf("Planes from %s is not 1: %u\n", filename, planes);
        return 0;
    }

    // read the bitsperpixel

    if ((i = fread(&bpp, 2, 1, file)) != 1) {
        printf("Error reading bpp from %s.\n", filename);
        return 0;
    }

    if (bpp != 24) {
        printf("Bpp from %s is not 24: %u\n", filename, bpp);
        return 0;
    }

    // seek past the rest of the bitmap header.

    fseek(file, 24, SEEK_CUR);

    // read the data.

    image->data = (char *) malloc(size);

    if (image->data == NULL) {
        printf("Error allocating memory for color-corrected image data");
        return 0;
    }

    if ((i = fread(image->data, size, 1, file)) != 1) {
        printf("Error reading image data from %s.\n", filename);
        return 0;
    }

    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
        temp = image->data[i];
        image->data[i] = image->data[i+2];
        image->data[i+2] = temp;
    }

    // we're done.

    return 1;

}


Image * loadTexture(char *filename){
    Image *image1;
    // allocate space for texture
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
		//exit(0);
    }

	if (!ImageLoad(filename, image1)) {
        //exit(1);
    }

    return image1;

}
// -----------------------------------------------------------------------------------------------


namespace GUI
{
	GUIData data;
}

void GUI::GUIData::initBoard(int w, int h)
{
	int size = getSize(w,h);
	w = getStartWidth(w,size);
	h = getStartHeight(h,size);
	_board = Board(w, h, size);
	_board.initBoard();
	_boardInit = true;
}

void GUI::GUIData::setBoardSize(int w, int h)
{
	int size = getSize(w,h);
	w = getStartWidth(w,size);
	h = getStartHeight(h,size);

	for(int i = 0; i < 8; ++i)
	{
		for (int j= 0; j < 8; j++)
		{
			_board.board[i][j]._height = size;
			_board.board[i][j]._width = size;
			_board.board[i][j]._posH = i * size + h;
			_board.board[i][j]._posW = j * size + w;
		}
	}
}

int GUI::GUIData::getSize(int w, int h)
{
	int size;
	if(w>h)
		size = h;
	else
		size = w;
	size /= 10;
	return size;
}

int GUI::GUIData::getStartWidth(int w, int size)
{
	int width = windowWidth/2;
	width-=size*4;	
	return width;
}

int GUI::GUIData::getStartHeight(int h, int size)
{
	int height = windowHeight/2;
	height-=size*4;
	return height;
}

void GUI::GUIData::displayText()
{
	glColor3f(1.0f,0.0f,0.0f);
	glRasterPos2f(300.0f,400.0f);
	string temp = "You are the red paddle!";
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*)temp.c_str());

}

/*GLuint GUI::loadTexture( const char * filename )
{

	 GLuint texture;

  int width, height;

  unsigned char * data;

  FILE * file;

 errno_t err = fopen_s(&file,filename,"rb");

  if ( file == NULL ) return 0;
  width = 1024;
  height = 512;
  data = (unsigned char *)malloc( width * height * 3 );
  //int size = fseek(file,);
  fread( data, width * height * 3, 1, file );
  fclose( file );

 for(int i = 0; i < width * height ; ++i)
{
   int index = i*3;
   unsigned char B,R;
   B = data[index];
   R = data[index+2];

   data[index] = R;
   data[index+2] = B;

}


glGenTextures( 1, &texture );
glBindTexture( GL_TEXTURE_2D, texture );
glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );


glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
free( data );

return texture;

}*/


void GUI::update()
{
	glutMainLoopEvent();
}

void GUI::special(int key, int i, int j)
{

}

void GUI::renderPieces(int i, int j)
{
	int size = data._board.board[i][j]._width / 10;
	int x = data._board.board[i][j]._posW + size;
	int y = data._board.board[i][j]._posH + size;

	size *= 8;

	glEnable(GL_TEXTURE_2D);
	switch (data._board.board[i][j]._player)
	{
	case PlayerType::P_TWO:
		switch (data._board.board[i][j]._piece)
		{
		case PieceType::BISHOP:
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			break;
		case PieceType::KING:
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			break;
		case PieceType::KNIGHT:
			glBindTexture(GL_TEXTURE_2D, texture[2]);
			break;
		case PieceType::PAWN:
			glBindTexture(GL_TEXTURE_2D, texture[3]);
			break;
		case PieceType::QUEEN:
			glBindTexture(GL_TEXTURE_2D, texture[4]);
			break;
		case PieceType::ROOK:
			glBindTexture(GL_TEXTURE_2D, texture[5]);
			break;
		default:
			break;
		}
		break;
	case PlayerType::P_ONE:
		switch (data._board.board[i][j]._piece)
		{
		case PieceType::BISHOP:
			glBindTexture(GL_TEXTURE_2D, texture[6]);
			break;
		case PieceType::KING:
			glBindTexture(GL_TEXTURE_2D, texture[7]);
			break;
		case PieceType::KNIGHT:
			glBindTexture(GL_TEXTURE_2D, texture[8]);
			break;
		case PieceType::PAWN:
			glBindTexture(GL_TEXTURE_2D, texture[9]);
			break;
		case PieceType::QUEEN:
			glBindTexture(GL_TEXTURE_2D, texture[10]);
			break;
		case PieceType::ROOK:
			glBindTexture(GL_TEXTURE_2D, texture[11]);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	


      glBegin( GL_QUADS );
      glTexCoord2d(0,1); glVertex2d(x,y);
      glTexCoord2d(0, 0); glVertex2d(x,y+size);
      glTexCoord2d(1,0); glVertex2d(x+size,y+size);
      glTexCoord2d(1,1); glVertex2d(x+size,y);
      glEnd();

	glDisable(GL_TEXTURE_2D);

}

void GUI::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if(data._board.board[i][j]._isBlack)
			{
				glColor3f(0.0f,0.0f,0.0f);
			}
			else
			{
				glColor3f(1.0f,1.0f,1.0f);
			}
			glBegin(GL_QUADS);
			glVertex2d(data._board.board[i][j]._posW,data._board.board[i][j]._posH);
			glVertex2d(data._board.board[i][j]._posW + data._board.board[i][j]._width,data._board.board[i][j]._posH);
			glVertex2d(data._board.board[i][j]._posW + data._board.board[i][j]._width,data._board.board[i][j]._posH + data._board.board[i][j]._height);
			glVertex2d(data._board.board[i][j]._posW,data._board.board[i][j]._posH + data._board.board[i][j]._height);
			glEnd();
			if(data._board.board[i][j]._player != PlayerType::NONE)
			{
				renderPieces(i, j);
			}
		}
	}
	glutSwapBuffers();
}

void GUI::changeViewport(int w, int h)
{	
	data.windowWidth = w;
	data.windowHeight = h;
	if(data._boardInit)
		data.setBoardSize(data.windowWidth, data.windowHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0,0,data.windowWidth,data.windowHeight);
	gluOrtho2D(0,data.windowWidth,data.windowHeight,0);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();

}

void GUI::prepareTextures(char* filename, int textureNum)
{
	Image *image1 = loadTexture(filename);
	if(image1 == NULL){

        printf("Image was not returned from loadTexture\n");

        //exit(0);

    }
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Create Texture
    glBindTexture(GL_TEXTURE_2D, texture[textureNum]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

void GUI::initTextures()
{
	glGenTextures(12, texture);
	prepareTextures("textures/BBishop.bmp", 0);
	prepareTextures("textures/BKing.bmp", 1);
	prepareTextures("textures/BKnight.bmp", 2);
	prepareTextures("textures/BPawn.bmp", 3);
	prepareTextures("textures/BQueen.bmp", 4);
	prepareTextures("textures/BRook.bmp", 5);
	prepareTextures("textures/WBishop.bmp", 6);
	prepareTextures("textures/WKing.bmp", 7);
	prepareTextures("textures/WKnight.bmp", 8);
	prepareTextures("textures/WPawn.bmp", 9);
	prepareTextures("textures/WQueen.bmp", 10);
	prepareTextures("textures/WRook.bmp", 11);
}

void GUI::initialize(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("BattleNet!");
	glClearColor(0.5f,0.5f,0.5f,1.0f);
	initTextures();
	glutReshapeFunc(&GUI::changeViewport);
	glutDisplayFunc(&GUI::render);
	glutSpecialFunc(&GUI::special);
	

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "GLEW Error");
	}
	data.initBoard(data.windowWidth, data.windowHeight);
}

const GUI::GUIData& GUI::GetData()
{
	 return data;
}

GUI::GUIData& GUI::MutableData()
{
	 return data;
}