#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

enum Color{
	White, // Initital 0
	Blue, // Player One
	Red, // Player Two
	Black //Explosion
};

char buffer[100];
int Record[5][6];
int Max[5][6];
Color color[5][6];
Color inputColor;

namespace playerOne
{
#include "project3.cpp"
};

namespace playerTwo
{
#include "student.cpp"
};

playerOne::Student p1;
playerTwo::Student p2;

void printBoard()
{
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<6;j++)
		{
			if(color[i][j]==Black) printf("\033[1;30mX\033[0m ");
			else if(color[i][j]==White) printf("0 ");
			else if(color[i][j]==Blue) printf("\033[1;34m%d\033[0m ",Record[i][j]);
			else printf("\033[1;31m%d\033[0m ",Record[i][j]);
		}
		puts("");
	}
}
bool checkEnd()
{
	for(int i=0;i<5;i++) for(int j=0;j<6;j++) if(color[i][j]==White||color[i][j]==inputColor) return false;
	return true;
}
void makeMove(int x, int y, Color clr)
{
	Record[x][y]++;
	color[x][y]=clr;
	if(Record[x][y]>=Max[x][y])
	{
		color[x][y]=Black;
		if(x-1>=0&&color[x-1][y]!=Black)
			makeMove(x-1,y,clr);
		if(x+1<5&&color[x+1][y]!=Black)
			makeMove(x+1,y,clr);
		if(y-1>=0&&color[x][y-1]!=Black)
			makeMove(x,y-1,clr);
		if(y+1<6&&color[x][y+1]!=Black)
			makeMove(x,y+1,clr);
	}
}

int main()
{
	memset(Record,0,sizeof(Record));
	for(int i=0;i<5;i++) for(int j=0;j<6;j++) color[i][j]=White;
	for(int i=0;i<5;i++) for(int j=0;j<6;j++) Max[i][j]=4-(i==0)-(i==4)-(j==0)-(j==5);
	while(1)
	{
		inputColor=Blue;
		printBoard();
		fgets(buffer,100,stdin);
		if(checkEnd())
		{
			puts("Player Two Win");
			break;
		}
		p1.makeMove(Record,Max,color,inputColor);
		makeMove(p1.getX(),p1.getY(),inputColor);
		inputColor=Red;
		printBoard();
		fgets(buffer,100,stdin);
		if(checkEnd())
		{
			puts("Player One Win");
			break;
		}
		p2.makeMove(Record,Max,color,inputColor);
		makeMove(p2.getX(),p2.getY(),inputColor);
	}
}
