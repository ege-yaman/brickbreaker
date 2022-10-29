// Deneme.cpp : Bu dosya 'main' işlevi içeriyor. Program yürütme orada başlayıp biter.
//

#include <iostream>
#include <windows.h>
#include <conio.h>

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorPosition;

using namespace std;

const int gameSpeed = 90;
const int width = 40;
const int height = 25;
int box[width][height];
char paint[width][height];

const int blank = 0;
const char s_blank = ' ';

const int wall = 1;
const char s_wall = '#';

const int fire = 9;
const char s_fire = '.';

const int ball = 5;
const char s_ball = 'o';
const int firstBallStartCoordinateX = 16;
const int firstBallStartCoordinateY = 20;

const int player = 2;
const char s_player = 'M';

const int brick = 3;
const char s_brick = '+';

const int flag = 4;


bool gameOver = false;
bool isWin = false;

int k = 0;


class Ball{
public:
	int posX;
	int posY;
	int dirX;
	int dirY;

	

	Ball(int x1, int y1, int x2, int y2)
	{
		posX = x1;
		posY = y1;
		dirX = x2;
		dirY = y2;
	}


};

class Stick{
public:
	int posX = 0;
	int posY = 0;
	int size = 0;

	Stick(int x1, int y1, int s)
	{
		posX = x1 - size/2;
		posY = y1;
		size = s;
	}

	
	Stick(int x1, int y1)
	{
		posX = x1 - 4;
		posY = y1;
		size = 8;
	}

};

class Brick {
public:
	bool broken = false;
	int posX = 0;
	int posY = 0;
	int sizeX = 1;
	int sizeY = 1;

	Brick(int x1, int y1, int x2, int y2)
	{
		posX = x1;
		posY = y1;
		sizeX = x2;
		sizeY = y2;
	}
	

};



Ball ball1(firstBallStartCoordinateX, firstBallStartCoordinateY, 1, -1);
Stick stick1(20, height - 1, 4);


Brick brick1(2, 5, 5, 2);
Brick brick2(8, 5, 5, 2);
Brick brick3(14, 5, 5, 2);
Brick brick4(20, 5, 5, 2);
Brick brick5(26, 5, 5, 2);
Brick brick6(32, 5, 5, 2);

Brick brick7(2, 8, 5, 2);
Brick brick8(8, 8, 5, 2);
Brick brick9(14, 8, 5, 2);
Brick brick10(20, 8, 5, 2);
Brick brick11(26, 8, 5, 2);
Brick brick12(32, 8, 5, 2);

Brick brick13(2, 11, 5, 2);
Brick brick14(8, 11, 5, 2);
Brick brick15(14, 11, 5, 2);
Brick brick16(20, 11, 5, 2);
Brick brick17(26, 11, 5, 2);
Brick brick18(32, 11, 5, 2);


Brick bricks[] = { brick1, brick2, brick3,  brick4, brick5, brick6, brick7, brick8, brick9, brick10,
brick11, brick12, brick13, brick14,  brick17, brick18 };

const int howManyBricks = sizeof(bricks) / sizeof(Brick);

int brickCounter = howManyBricks;


void GetInput()
{
	char key = _getch();

	if (key == 'a' && stick1.posX > 1)
		stick1.posX -= 1;
	else if (key == 'd'&& stick1.posX < width - stick1.size - 1)
		stick1.posX += 1;

}

void GoToPose(int x, int y)
{
	cursorPosition.X = x;
	cursorPosition.Y = y;
	SetConsoleCursorPosition(console, cursorPosition);
}

void ClearScreen()
{
	cursorPosition;	cursorPosition.X = 0;	cursorPosition.Y = 0;	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void SetWalls()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i == 0|| j == 0 || j == width - 1)
				box[j][i] = wall;
			else if (i == height - 1)
			{
				box[j][i] = fire;
			}
			else
				box[j][i] = blank;
		}
	}
}

void DestroyBrick(int x1, int y1)
{
	box[x1][y1] = flag;
}

void CheckBlock(Brick* _brick)
{
	for (int i = 0; i < _brick->sizeY; i++)
	{
		for (int j = 0; j < _brick->sizeX; j++)
		{
			if (box[_brick->posX + j][_brick->posY + i] == flag)
			{
				_brick->broken = true;
			}
		}
	}


}



void MoveBall(Ball* _ball)
{
	box[_ball->posX][_ball->posY] = ball;
	
	//collide with wall or player stick
	if (box[_ball->posX + _ball->dirX][_ball->posY] == wall || box[_ball->posX + _ball->dirX][_ball->posY] == player)
	{
		_ball->dirX *= -1;
	}
	if (box[_ball->posX][_ball->posY + _ball->dirY] == wall || box[_ball->posX][_ball->posY + _ball->dirY] == player)
	{
		_ball->dirY *= -1;
	}
	
	if (box[_ball->posX + _ball->dirX][_ball->posY + _ball->dirY] == wall || box[_ball->posX + _ball->dirX][_ball->posY + _ball->dirY] == player)
	{
		_ball->dirX *= -1;
		_ball->dirY *= -1;
	}

	//collider with fire
	if (box[_ball->posX + _ball->dirX][_ball->posY] == fire)
	{
		gameOver = true;
	}
	if (box[_ball->posX][_ball->posY + _ball->dirY] == fire)
	{
	    gameOver = true;
	}

	if (box[_ball->posX + _ball->dirX][_ball->posY + _ball->dirY] == fire)
	{
		gameOver = true;
	}

	//collide with bricks
	if (box[_ball->posX + _ball->dirX][_ball->posY] == brick)
	{
		DestroyBrick(_ball->posX + _ball->dirX, _ball->posY);
		_ball->dirX *= -1;
	}
	if (box[_ball->posX][_ball->posY + _ball->dirY] == brick)
	{
		DestroyBrick(_ball->posX, _ball->posY + _ball->dirY);
		_ball->dirY *= -1;
	}

	if (box[_ball->posX + _ball->dirX][_ball->posY + _ball->dirY] == brick)
	{
		DestroyBrick(_ball->posX += _ball->dirX, _ball->posY + _ball->dirY);
		_ball->dirX *= -1;
		_ball->dirY *= -1;
	}

	//move
	if (!gameOver && k == 6)
	{
		_ball->posX += _ball->dirX;
		_ball->posY += _ball->dirY;

		k = 0;
	}

	k++;
}

void RenderBrick(Brick* _brick)
{
	for (int i = 0; i < _brick->sizeY; i++)
	{
		for (int j = 0; j < _brick->sizeX; j++)
		{
			box[_brick->posX + j][_brick->posY + i] = brick;
		}
	}
}

void MoveStick(Stick* _stick)
{
	for (int i = 0; i < _stick->size; i++)
	{
		box[_stick->posX + i][_stick->posY] = player;
	}


}

void SetClean()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			paint[j][i] = ' ';
		}
	}
}





void Update()
{
	bool weHaveBricks = false;

	if (_kbhit())
	{
		GetInput();
	}


	SetWalls();

	for (int i = 0; i < howManyBricks; i++)
	{
		if (!bricks[i].broken)
		{
			RenderBrick(&bricks[i]);
			weHaveBricks = true;
		}
	}

	if (!weHaveBricks)
	{
		gameOver = true;
		isWin = true;
	}
	MoveStick(&stick1);

	MoveBall(&ball1);

	for(int i = 0; i < brickCounter; i++)
	CheckBlock(&bricks[i]);



}

void DrawBackground()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
				paint[j][i] = s_wall;
			else
				paint[j][i] = ' ';	
		}
	}
}



void Draw()
{
	string screenText = "";


	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			switch (box[j][i])
			{
			case 0:
				paint[j][i] = s_blank;
				break;
			case 1:
				paint[j][i] = s_wall;
				break;
			case 2:
				paint[j][i] = s_player;
				break;
			case 3:
				paint[j][i] = s_brick;
				break;
			case 9:
				paint[j][i] = s_fire;
				break;
			case 5:
				paint[j][i] = s_ball;
				break;
			default:
				paint[j][i] = s_blank;

			}


		}
	}



	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			screenText += paint[j][i];
			screenText += " ";
		}

	
		screenText += '\n';
	}

	cout << screenText << endl;
}

bool goPlay;

void MainMenu()
{
	cout << "press any key to start" << endl;

	while (1)
	{
		if (_kbhit())
		{
			break;
		}
	}


}


int main()
{
	MainMenu();

	ClearScreen();

	SetClean();
	
	
	while (1)
	{
	    Update();

	    Draw();

	    Sleep(1000 / gameSpeed);

	    if (gameOver)
	        break;

	    ClearScreen();



    }
	

	system("cls");

	if (isWin)
		cout << "WELL DONE!!" << endl;
	else
		cout << "Game Over" << endl;
	
	cout << '\n';

	
	cout << "press 'q' to quit";
	
	
	while (1)
	{
		if (_kbhit())
		{
			if(_getch() == 'q')
			break;
		}
	}


	return 0;
}







// Programı çalıştır: Ctrl + F5 veya Hata Ayıkla > Hata Ayıklamadan Başlat menüsü
// Programda hata ayıkla: F5 veya Hata Ayıkla > Hata Ayıklamayı Başlat menüsü

// Kullanmaya Başlama İpuçları: 
//   1. Dosyaları eklemek/yönetmek için Çözüm Gezgini penceresini kullanın
//   2. Kaynak denetimine bağlanmak için Takım Gezgini penceresini kullanın
//   3. Derleme çıktısını ve diğer iletileri görmek için Çıktı penceresini kullanın
//   4. Hataları görüntülemek için Hata Listesi penceresini kullanın
//   5. Yeni kod dosyaları oluşturmak için Projeye Git > Yeni Öğe ekle veya varolan kod dosyalarını projeye eklemek için Proje > Var Olan Öğeyi Ekle adımlarını izleyin
//   6. Bu projeyi daha sonra yeniden açmak için Dosya > Aç > Proje'ye gidip .sln uzantılı dosyayı seçin
