#include<iostream>
#include<windows.h>
#include<conio.h>
#include<math.h>
using namespace std;

const int Up_Arrow = 72, Down_Arrow = 80, Left_Arrow = 75, Right_Arrow = 77;
enum Direction { UP, DOWN, LEFT, RIGHT };
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cursor);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
struct Pos {
	int ri, ci;
};
#define MSS 100
#define Rows  35
#define Cols  90
#define MaxS 2 //Max no of snakes in game
struct Snake {

	Pos Ps[MSS];
	int Size;
	char SSym;
	int score;
	bool GameOver;
	Direction DIR = RIGHT; 
};
void FoodGeneration(Pos& Food, Snake S[], char Symbol);
void Init(Snake S[], Pos& Food, char FoodSym)
{
	S[0].Size = 3;
	S[1].Size = 3;
	for (int i = 0; i < S[0].Size; i++)
	{
		S[0].Ps[i].ri = Rows / 2;
		S[0].Ps[i].ci = Cols / 2 - i;
		S[1].Ps[i].ri = Rows / 4;
		S[1].Ps[i].ci = Cols / 4 - i;
		//You can make it more generic by adding new loop and setting the position of ith snake in loop

	}
	S[0].SSym = '*'; //you can also use loop here to set the symbols of snakes or you can take input from user
	S[1].SSym = -37;
	S[0].score = 0; //score of players at start
	S[1].score = 0;
	FoodGeneration(Food, S, FoodSym);
	S[0].GameOver = false;
	S[1].GameOver = false;
}
void SnakeDisplay(Snake S[])
{
	//Here you can also use loop to iterate thorugh the snakes and dislpay them instead of displaying each snake
	for (int i = 0; i < S[0].Size; i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
		gotoRowCol(S[0].Ps[i].ri, S[0].Ps[i].ci);
		cout << S[0].SSym;
	}
	for (int i = 0; i < S[1].Size; i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
		gotoRowCol(S[1].Ps[i].ri, S[1].Ps[i].ci);
		cout << S[1].SSym;
	}
}
void SnakeErase(Snake S[])
{
	//Here you can also use loop to iterate thorugh the snakes and erase them instead of each snake
	for (int i = 0; i < S[0].Size; i++)
	{
		gotoRowCol(S[0].Ps[i].ri, S[0].Ps[i].ci);
		cout << " ";
	}
	for (int i = 0; i < S[1].Size; i++)
	{
		gotoRowCol(S[1].Ps[i].ri, S[1].Ps[i].ci);
		cout << " ";
	}

}
void ChangeDirection(Snake s[])
{
	//Here you have to think and implement different keys with the direction of multiple snakes by yourself because it solely depends on the programmer and user
		char ch = _getch();
		int key = int(ch);
		if (key == -32)
		{
			key = _getch();
			switch (key)
			{
			case Up_Arrow:
				if (s[0].DIR != DOWN)
				{
					s[0].DIR = UP;
				}

				break;
			case Down_Arrow:
				if (s[0].DIR != UP)
				{
					s[0].DIR = DOWN;
				}
				break;
			case Left_Arrow:
				if (s[0].DIR != RIGHT)
				{
					s[0].DIR = LEFT;
				}

				break;
			case Right_Arrow:
				if (s[0].DIR != LEFT)
				{
					s[0].DIR = RIGHT;
				}

				break;
			}
		}
		else if (key == 'D' or key == 'd')
		{
			if (s[1].DIR != LEFT)
			{
				s[1].DIR = RIGHT;
			}

		}
		else if (key == 'W' or key == 'w')
		{
			if (s[1].DIR != DOWN)
			{
				s[1].DIR = UP;
			}
		}
		else if (key == 'a' or key == 'A')
		{
			if (s[1].DIR != RIGHT)
			{
				s[1].DIR = LEFT;
			}
		}
		else if (key == 'S' or key == 's')
		{
			if (s[1].DIR != UP)
			{
				s[1].DIR = DOWN;
			}
		}
		else if (key == 32)
		{
			gotoRowCol(36, 20);
			cout << "Game is paused...\n";
			gotoRowCol(37, 20);
			system("Pause");
			gotoRowCol(36, 20);
			cout << "                                     ";
			gotoRowCol(37, 20);
			cout << "                                     ";
		}
}
void ChangeSnakeDirection(Direction DIR, Snake &S)
{
	switch (DIR)
	{
	case UP:
		S.Ps[0].ri--;
		if (S.Ps[0].ri == -1)
			S.Ps[0].ri = Rows - 1;
		break;
	case DOWN:
		S.Ps[0].ri++;
		if (S.Ps[0].ri == Rows)
			S.Ps[0].ri = 0;
		break;
	case LEFT:
		S.Ps[0].ci--;
		if (S.Ps[0].ci == -1)
			S.Ps[0].ci = Cols - 1;
		break;
	case RIGHT:
		S.Ps[0].ci++;
		if (S.Ps[0].ci == Cols)
			S.Ps[0].ci = 0;
		break;
	}
}
void MoveSnake(Snake S[])
{
	for (int si = 0; si < 2; si++)
	{
		for (int i = S[si].Size - 1; i >= 1; i--)
		{
			S[si].Ps[i] = S[si].Ps[i - 1];
		}
		ChangeSnakeDirection(S[si].DIR, S[si]);
	}
}
bool isValidFOodLocation(Pos Food, Snake S[])
{
	//Here you can also use loop to iterate thorugh the snakes and check for this instead of each snake
	for (int i = 0; i < S[0].Size; i++)
	{
		if (Food.ri == S[0].Ps[i].ri && Food.ci == S[0].Ps[i].ci)
			return false;
	}
	for (int i = 0; i < S[1].Size; i++)
	{
		if (Food.ri == S[1].Ps[i].ri && Food.ci == S[1].Ps[i].ci)
			return false;
	}
	return true;
}
void FoodGeneration(Pos& Food, Snake S[], char Symbol) {
	do {
		Food.ri = rand() % Rows;
		Food.ci = rand() % Cols;
	} while (!isValidFOodLocation(Food, S) && !isValidFOodLocation(Food, S));
	gotoRowCol(Food.ri, Food.ci);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	cout << Symbol;
}
bool IsFoodEaten(Pos Food, Snake S[],int &index) {
	//Here you can also use loop to iterate thorugh the snakes and check them instead of checking each snake
	if (Food.ri == S[0].Ps[0].ri && Food.ci == S[0].Ps[0].ci) {
		index = 0;
		return true;
	}
	if (Food.ri == S[1].Ps[0].ri && Food.ci == S[1].Ps[0].ci)
	{
		index = 1;
		return true;
	}
	return false;
}
bool IsBonusFoodEaten(Pos Food, Snake S[]) {
	return ((Food.ri == S[0].Ps[0].ri && Food.ci == S[0].Ps[0].ci)||( Food.ri == S[1].Ps[0].ri && Food.ci == S[1].Ps[0].ci));
}
bool KillItSelf(Snake S[]) {
	//Here you can also use loop to iterate thorugh the snakes and check them instead of going through each snake
	for (int i = 1; i < S[0].Size; i++)
	{
		if (S[0].Ps[0].ri == S[0].Ps[i].ri && S[0].Ps[0].ci == S[0].Ps[i].ci) {
			return true;
		}
	}
	for (int i = 1; i < S[1].Size; i++)
	{
		if (S[1].Ps[0].ri == S[1].Ps[i].ri && S[1].Ps[0].ci == S[1].Ps[i].ci) {
			return true;
		}
	}
	return false;
}
bool Collision(Snake S[]) {
	//Checking if two or more snake collides with itself
	for (int F = 0; F < 2; F++)
	{
		for (int Sn = 0; Sn < 2; Sn++)
		{
			if (Sn != F)
			{
				for (int i = 0; i < S[Sn].Size; i++)
				{

					if (S[F].Ps[0].ri == S[Sn].Ps[i].ri && S[F].Ps[0].ci == S[Sn].Ps[i].ci)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}
void Main() {
	int index;
	int SnakeSpeed = 100;
	char FSym = 'o';
	Snake S[MaxS];
	Pos Food;
	Init(S, Food, FSym);
	//Direction DIR = RIGHT;
	bool flag = false;
	int BonusCounter = 1;
	bool bonus = false;
	do {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		HideCursor();
		gotoRowCol(36, 94);
		cout << "SCORE 1: " << S[0].score;
		gotoRowCol(37, 94);
		cout << "SCORE 2: " << S[1].score;
		if (_kbhit())
		{
			ChangeDirection(S);
		}
		else {
			SnakeErase(S);
			MoveSnake(S);
			SnakeDisplay(S);
			if (IsFoodEaten(Food, S, index))
			{
				gotoRowCol(36, 40);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
				cout  <<"               ";
				if (bonus) {
					BonusCounter = 1;
					S[index].score += 5;
					S[index].Size += 5;
				}
				else
					S[index].score += 1;
				if (S[index].score % 3 == 0 && SnakeSpeed != 10)
					SnakeSpeed -= 10;
				BonusCounter++;
				if (BonusCounter == 5)
				{
					gotoRowCol(36, 40);
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					cout << "Bonus Food Time";
					char BonusSym = '$';
					FoodGeneration(Food, S, BonusSym);
					
				}
				else
				{
					S[index].Size += 2;
					FoodGeneration(Food, S, FSym);
				}
			}
			if (BonusCounter == 5)
				bonus = true;
			else
				bonus = false;
			Sleep(SnakeSpeed);
			
			if (KillItSelf(S) || Collision(S))
			{
				system("cls");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
				cout << "\n\n\t\t\t------>Game Over<------" << endl;
				cout << "\n\n\t\t\t----->Score Board<-----" << endl;
				if (S[0].score > S[1].score) {
					cout << "\t\t\t     Score of Snake 1 is: " << S[0].score << endl;
					cout << "\t\t\t     Score of Snake 2 is: " << S[1].score << endl;
				}
				else if (S[1].score > S[0].score) {
					cout << "\t\t\t     Score of Snake 2 is: " << S[1].score << endl;
					cout << "\t\t\t     Score of Snake 1 is: " << S[0].score << endl;
				}
				else if (S[1].score = S[0].score) {
					cout << "\t\t\t     Score of Snake 1 is: " << S[0].score << endl;
					cout << "\t\t\t     Score of Snake 2 is: " << S[1].score << endl;
				}
				break;
			}
		}
	} while (true);
}
int main()
{
	Main();
}