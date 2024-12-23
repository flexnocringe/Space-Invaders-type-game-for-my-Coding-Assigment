#include <iostream>
#include <iomanip>
#include <cmath>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>
#include <typeinfo>
#include <conio.h>
#include <atomic>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
using namespace std;
class Bullet
{
protected:
	int x_bullet, y_bullet, bullet_fly;
	bool shoot = 0;
	char bullet = '|';
public:
	void Shoot(int x, int y)
	{
		x_bullet = x;
		y_bullet = y;
		shoot = 1;
	}
	int Bullet_fly(int dir)
	{
		if (dir == 1)
		{
			x_bullet--;
		}
		else if (dir == -1)
		{
			x_bullet++;
		}
		return x_bullet;
	}
	int Get_y_bullet()
	{
		return y_bullet;
	}
	char Get_bullet()
	{
		return bullet;
	}
	bool Get_shoot()
	{
		return shoot;
	}
};
class Ship : public Bullet
{
private:
	int x, y;
	char ship = 'W';
public:
	Ship() : x(17), y(23) {}
	~Ship() {}
	int Get_x()
	{
		return x;
	}
	int Get_y()
	{
		return y;
	}
	char Get_ship()
	{
		return ship;
	}
	void operator ++()
	{
		if (y < 43)
		{
			y++;
		}
	}
	void operator --()
	{
		if (y > 1)
		{
			y--;
		}
	}
};
class Alien : public Bullet
{
private:
	int pts, x, y, start_x;
	char alien;
public:

	Alien(int x, int y, int start_x, int pts, char alien) : x(x), y(y), start_x(start_x), pts(pts), alien(alien) {}
	~Alien() {}
	int Get_pts()
	{
		return pts;
	}
	int Get_x()
	{
		return x;
	}
	int Get_y()
	{
		return y;
	}
	char Get_alien()
	{
		return alien;
	}
	void operator ++()
	{
		
		if (x < 6+start_x)
		{
			x++;
		}	
	}
	void operator ++(int)
	{
		if (y < 42)
		{
			y++;
		}
		
	}
	void operator --(int)
	{
		if (y > 3)
		{
			y--;
		}
		
	}
};
void Map(char Grid[21][45])
{
	ifstream F("Map.txt");
	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 46; j++)
		{
			F.get(Grid[i][j]);
		}
	}
}
void Display_map(char Grid[21][45])
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 45; j++)
		{
			cout << Grid[i][j];
		}
		cout << endl;
	}
}
void Display_score(int lives, int lvl, int score, map<char, int> board)
{
	map<char, int> ::iterator it;
	string line;
	line.resize(45);
	fill(line.begin(), line.end(), '*');

	cout <<endl<<setw(10)<<"Lives: "<<lives<<setw(12)<<"Level: "<<lvl<<setw(12) << "Score: " << score << endl<<endl;
	cout << line<<endl<<endl;
	for (it = board.begin(); it != board.end(); it++)
	{
		cout<<"     " << it->first << " - " << it->second;
	}
	cout<<endl<<endl<< "************** Press Q to Exit **************";
}
atomic <int> x=9, y=22, x_bullet, y_bullet;
atomic <bool> shoot=0, Quit=0, Play=0;
atomic <char> player = {}, bullet = {};
void Input()
{
	Ship ship;
	char key_press;
	bool Exit = Quit;
	while (!Exit)
	{
		if (Quit)
		{
			Exit = 1;
			break;
		}
		else if (!Quit)
		{
			key_press = _getch();
			if (key_press == 'a')
			{
				--ship;
			}
			else if (key_press == 'd')
			{
				++ship;
			}
			else if (key_press == 's' && !shoot)
			{
				ship.Shoot(x, y);
				shoot = ship.Get_shoot();
				y_bullet = ship.Get_y_bullet();
				x_bullet = ship.Bullet_fly(1);
				bullet = ship.Get_bullet();
			}
			x = ship.Get_x();
			y = ship.Get_y();
			player = ship.Get_ship();
			if (key_press == 'p')
			{
				Play = 1;
				Exit = 1;
			}
			if (key_press == 'q')
			{
				Quit = 1;
				Exit = 1;
			}
			if (key_press == 0x20)
			{
				break;
			}
		}
	}
}
Alien Rng_enemy(int x, int y, int rng)
{
	char a1 = 'O', a2 = '8', a3 = 'V', a4 = 'M';
	int p1 = 25, p2 = 50, p3 = 100, p4 = 200;
	if (rng == 1)
	{
		Alien alien(x, y, x, p1, a1);
		return alien;
	}
	else if (rng == 2)
	{
		Alien alien(x, y, x , p2, a2);
		return alien;
	}
	else if (rng == 3)
	{
		Alien alien(x, y, x, p3, a3);
		return alien;
	}
	else if (rng == 4)
	{
		Alien alien(x, y, x, p4, a4);
		return alien;
	}
}
void Main_menu(bool& play)
{
	thread input(Input);
	char menu[45];
	ifstream F("Menu.txt");
	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 45; j++)
		{
			F.get(menu[j]);
			cout << menu[j];
		}
	}
	input.join();
	play = Play;
}
void Level_passed(int& lvl, int& score, int& lives, map<char, int> board)
{
	system("CLS");
	thread input(Input);
	cout << "*********************************************" << endl << endl;
	cout << "        Good Job!!! You Passed Level " << lvl << endl << endl;
	cout << "*********************************************" << endl;
	Display_score(lives, lvl, score, board);
	cout << endl << "************ Press P to Continue ************";
	input.join();
}
void Game_over(int& lvl, int& score, int& lives, map<char, int> board)
{
	system("CLS");
	cout << "*********************************************" << endl << endl;
	cout << "      Game Over!!! You Ran Out Of Ships      " << endl << endl;
	cout << "*********************************************" << endl;
	Display_score(lives, lvl, score, board);
}
void Game_end(int& lvl, int& score, int& lives, map<char, int> board)
{
	system("CLS");
	cout << "*********************************************" << endl << endl;
	cout << "          Thank You For Playing!!!           " << endl << endl;
	cout << "*********************************************" << endl;
	Display_score(lives, lvl, score, board);
}
void Main_game(int& lvl, int& score, int& lives, map<char,int>& board)
{
	thread mov(Input);
	vector<Alien> alien;
	char Grid[21][45] = {};
	int j = 1, l = 5, rng;
	srand(time(NULL));
	for (int i = 0; i < 10 * lvl; i++)
	{
		rng = rand() % 4 + 1;
		alien.push_back(Rng_enemy(j, l, rng));
		if (l < 32)
		{
			l += 3;
		}
		else
		{
			l = 5;
			j += 2;
		}
	}
	Map(Grid);
	int x1 = 9, y1 = 22, x_bullet1 = 1, x_bullet2=1, y_bullet1 = 1, y_bullet2=1, rng_shoot;
	char bullet2;
	bool shoot1;
	bool shoot2=0;
	bool game = 5;
	bool exit = 0;
	bool lborder=0;
	unsigned char delay = 0;
	srand(time(NULL));
	while (game)
	{
		delay++;
		shoot1 = shoot;
		x1 = x;
		y1 = y;
		exit = Quit;
		if (!shoot2 && !alien.empty())
		{
			rng_shoot = rand() % alien.size() * 4 + 1;
			if (rng_shoot <= alien.size())
			{
				shoot2 = 1;
				alien[rng_shoot - 1].Shoot(alien[rng_shoot - 1].Get_x(), alien[rng_shoot - 1].Get_y());
				x_bullet2 = alien[rng_shoot - 1].Bullet_fly(-1);
				y_bullet2 = alien[rng_shoot - 1].Get_y_bullet();
				bullet2 = alien[rng_shoot - 1].Get_bullet();
			}
		}
		system("CLS");
		for (int i = 0; i < alien.size(); i++)
		{
			Grid[alien[i].Get_x()][alien[i].Get_y()] = alien[i].Get_alien();
		}
		Grid[x1][y1] = player;
		if (shoot2)
		{
			x_bullet2++;
			if (Grid[x_bullet2 + 1][y_bullet2] == '*')
			{
				shoot2 = 0;
			}
			else if (Grid[x_bullet2 + 1][y_bullet2] == '#')
			{
				shoot2 = 0;
				Grid[x_bullet2 + 1][y_bullet2] = ' ';
			}
			else if (Grid[x_bullet2 + 1][y_bullet2] == 'W')
			{
				lives--;
				shoot2 = 0;
			}
			Grid[x_bullet2][y_bullet2] = bullet2;
		}
		if (shoot1)
		{
			x_bullet1 = x_bullet;
			x_bullet--;
			y_bullet1 = y_bullet;
			if (Grid[x_bullet1 - 1][y_bullet1] == '*')
			{
				shoot = 0;
				shoot1 = 0;
			}
			else if (Grid[x_bullet1 - 1][y_bullet1] == '#')
			{
				shoot = 0;
				shoot1 = 0;
				Grid[x_bullet1 - 1][y_bullet1] = ' ';
			}
			else if (Grid[x_bullet1 - 1][y_bullet1] == '|')
			{
				shoot = 0;
				shoot1 = 0;
				shoot2 = 0;
				Grid[x_bullet1 - 1][y_bullet1] = ' ';
			}
			for (int i = 0; i < alien.size(); i++)
			{
				if (x_bullet1 == alien[i].Get_x() && y_bullet1 == alien[i].Get_y())
				{
					shoot = 0;
					shoot1 = 0;
					score += alien[i].Get_pts();
					board.insert(pair<char, int>(alien[i].Get_alien(), 0));
					board.at(alien[i].Get_alien())++;
					alien.erase(alien.begin() + i);
					break;
				}
			}
			Grid[x_bullet1][y_bullet1] = bullet;
		}
		Display_map(Grid);
		Display_score(lives, lvl, score, board);
		Grid[x1][y1] = ' ';
		for (int i = 0; i < alien.size(); i++)
		{
			Grid[alien[i].Get_x()][alien[i].Get_y()] = ' ';
		}
		Grid[x_bullet1][y_bullet1] = ' ';
		Grid[x_bullet2][y_bullet2] = ' ';
		if (delay % 8 == 0)
		{
			if (!lborder)
			{
				for (int i = 0; i < alien.size(); i++)
				{
					alien[i]--;
					if (!lborder)
					{
						for (int j = 0; j < alien.size(); j++)
						{
							if (alien[i].Get_y() == 3)
							{
								lborder = 1;
								break;
							}
						}
					}
				}
				if (lborder)
				{
					for (int i = 0; i < alien.size(); i++)
					{
						++alien[i];
					}
				}
			}
			else if (lborder)
			{
				for (int i = 0; i < alien.size(); i++)
				{
					alien[i]++;
					if (lborder)
					{
						for (int j = 0; j < alien.size(); j++)
						{
							if (alien[i].Get_y() == 41)
							{
								lborder = 0;
								break;
							}
						}
					}
				}
				if (!lborder)
				{
					for (int i = 0; i < alien.size(); i++)
					{
						++alien[i];
					}
				}
			}
		}
		if (exit == 1)
		{
			game = 0;
			mov.join();
		}
		else if (alien.empty())
		{
			game = 0;
			mov.join();
		}
		else if (lives == 0)
		{
			game = 0;
			mov.join();
		}
	}
}
int main()
{
	int lvl = 1;
	int score = 0;
	int lives = 1;
	bool play = 0;
	map<char, int> board;
	Main_menu(play);
	if (play)
	{
		for (int i = 0; i < 3; i++)
		{
			Main_game(lvl, score, lives, board);
			if (lives == 0)
			{
				Game_over(lvl, score, lives, board);
				break;
			}
			else if (lvl == 3 || Quit==1)
			{
				Game_end(lvl, score, lives, board);
				break;
			}
			else if (lives > 0)
			{
				Level_passed(lvl, score, lives, board);
				lvl++;
			}
		}
	}
	return 0;
}