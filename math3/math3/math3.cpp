#include "pch.h"
#include <time.h>
#include <iostream>
#include <string>
using namespace std;

struct gem;
gem **arrgenerations(size_t X, size_t Y);
void create_random_color(gem &element);
void init(gem **Array, size_t X, size_t Y);
void move(gem **Array, unsigned int x1, unsigned int y1, char m, size_t X, size_t Y);
void offset_crystal_removal(gem **Array, size_t X, size_t Y, unsigned int sum);
bool search_thee(gem **Array, size_t X, size_t Y);
void logik(gem **Array, size_t X, size_t Y);
void dump(gem **Array, size_t X, size_t Y);
void tick(gem **Array, size_t X, size_t Y, int &A);

int actions = 0;

struct gem
{
	char color = '*';
	//char special = NULL;                   //usless
	bool destroy = false;
};

gem **arrgenerations(size_t X, size_t Y)
{
	gem **newarr = new gem*[X];
	for (size_t i = 0; i < X; i++)
	{
		newarr[i] = new gem[Y];
	}
	return newarr;
}

//char create_random_special()                     //usless
//{
//	if ((rand() % 10)==1)
//	{
//		return (char)(rand() % 6 + 65);
//	}
//}

void create_random_color(gem &element)
{
	element.color = (char)(rand() % 6 + 65);
	element.destroy = false;
}

void init(gem **Array, size_t X, size_t Y)
{
	for (size_t i = 0; i < X; i++)
	{
		for (size_t j = 0; j < Y; j++)
		{
			create_random_color(Array[i][j]);
		}
	}
}

void tick(gem **Array, size_t X, size_t Y, int &A)
{
	dump(Array, X, Y);
	A++;
}
void move(gem **Array, unsigned int x1, unsigned int y1, char m, size_t X, size_t Y)
{
		switch (m)
		{
		case 'l':
			swap(Array[x1][y1], Array[x1][y1 - 1]);
			if (!search_thee(Array, X, Y))
			{
				swap(Array[x1][y1], Array[x1][y1 - 1]);
				std::cout << "Unsuitable combination" << std::endl;
			}
			else
			{
				tick(Array, X, Y, actions);
			}
			break;
		case 'r':
			swap(Array[x1][y1], Array[x1][y1 + 1]);
			if (!search_thee(Array, X, Y))
			{
				swap(Array[x1][y1], Array[x1][y1 + 1]);
				std::cout << "Unsuitable combination" << std::endl;
			}
			else
			{
				tick(Array, X, Y, actions);
			}
			break; 
		case 'u':
			swap(Array[x1][y1], Array[x1 - 1][y1]);
			if (!search_thee(Array, X, Y))
			{
				swap(Array[x1][y1], Array[x1 - 1][y1]);
				std::cout << "Unsuitable combination" << std::endl;
			}
			else
			{
				tick(Array, X, Y, actions);
			}
			break;
		case 'd':
			swap(Array[x1][y1], Array[x1 + 1][y1]);
			if (!search_thee(Array, X, Y))
			{
				swap(Array[x1][y1], Array[x1 + 1][y1]);
				std::cout << "Unsuitable combination" << std::endl;
			}
			else
			{
				tick(Array, X, Y, actions);
			}
			break;
		default:
			std::cout << "Unsuitable combination" << std::endl;
			return;
			break;
		}
}
//void mix();

void offset_crystal_removal(gem **Array, size_t X, size_t Y, unsigned int sum)
{
	int h = 0;
	while (h!=sum)
	{
		for (size_t i = 0; i < X; i++)
			{
				for (size_t j = 0; j < Y; j++)
				{
					if (Array[i][j].destroy)
					{
						if (i==0)
						{
							create_random_color(Array[i][j]);
							//action = tick(Array, X, Y, action);
							h++;
						}
						else
						{
							swap(Array[i][j], Array[i - 1][j]);
							//action = tick(Array, X, Y, action);
							std::cout << "\nkristal move for delete "<<i<<" "<<j<<" in "<< i-1 <<" "<<j<<std::endl;
						}					
					}
				}
			}
	}
}

bool search_thee(gem **Array, size_t X, size_t Y)
{
	bool flag = false;
	for (size_t i = 0; i < X; i++)         // search lines
	{
		for (size_t j = 0; j < Y-2; j++)
		{
			if (Array[i][j].color == Array[i][j + 1].color && Array[i][j + 1].color == Array[i][j + 2].color)
			{
				flag = true;
				for (size_t k = j + 3; k < Y; k++)
				{
					if (Array[i][j].color == Array[i][k].color)
					{
						std::cout << "\nkristal destroy " << i << " " << j<<std::endl;
						Array[i][k].destroy = true;
					}
					else
					{
						break;
					}
				}
				Array[i][j].destroy = Array[i][j + 1].destroy = Array[i][j + 2].destroy = true;
				std::cout << "\nkristal destroy " << i << " " << j <<" : " << i << " " << j+1 << " : " << i << " " << j+2 << std::endl;
			}
		}
	}
	for (size_t j = 0; j < Y; j++)         // search columns
	{
		for (size_t i = 0; i < X - 2; i++)
		{
			if (Array[i][j].color == Array[i + 1][j].color && Array[i + 1][j].color == Array[i + 2][j].color)
			{
				flag = true;
				for (size_t k = i + 3; k < X; k++)
				{
					if (Array[i][j].color == Array[k][j].color)
					{
						Array[k][j].destroy = true;
						std::cout << "\nkristal destroy " << i << " " << j << std::endl;
					}
					else
					{
						break;
					}
				}
				Array[i][j].destroy = Array[i + 1][j].destroy = Array[i + 2][j].destroy = true;
				std::cout << "\nkristal destroy " << i << " " << j << " : " << i+1 << " " << j  << " : " << i+2 << " " << j << std::endl;
			}
		}
	}
	if (flag)
	{
		int sum = 0;
		for (size_t i = 0; i < X; i++)
		{
			for (size_t j = 0; j < Y; j++)
			{
				if (Array[i][j].destroy)
				{
					sum++;
				}
			}
		}
		offset_crystal_removal(Array, X, Y, sum);
		return true;
	}
	else
	{
		return false;
	}
}

void logik(gem **Array, size_t X, size_t Y)
{
	if (search_thee(Array, X, Y))
	{
		logik(Array, X, Y);
	}
}

void dump(gem **Array, size_t X, size_t Y)
{
	std::cout << " y0123456789\nx|----------\n";
	for (size_t i = 0; i < X; i++)
	{
		std::cout << i<<"|";
		for (size_t j = 0; j < Y; j++)
		{
			std::cout << Array[i][j].color;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void play(gem **Array, size_t X, size_t Y)
{
	char action = '\0';
	char m = '\0';
	unsigned int x1 = -1;
	unsigned int y1 = -1;
	std::cout << " Have a nice game\n Enter 'q' to exit\nexample input for move - m x y d \n x, у - coordinates(0 - 9)\nd - movement in(lrud - left, right, up, down)\n\n";
	while (true)
	{
		std::cin >> action;
		if (action =='m')
		{
			std::cin >> x1 >> y1 >> m;
			if (10 > x1 && x1 >= 0 && 10 > y1 && y1 >= 0 && (m == 'l' && y1 != 0 || m == 'r' && y1 != 9 || m == 'u'&& x1 != 0 || m == 'd'&& x1 != 9))
			{
				for (int i = 0; i < 25; i++)
				{
					std::cout << std::endl;
				}
				move(Array, x1, y1, m, X, Y);
			}
			else
			{
				std::cout << "\nERROR wrong command call, see definition 'move'\n" << std::endl;
				return;
			}
		}	
		else if (action == 'q')
		{
			std::cout << "Exit, good luck\n";
			return;
		}	
		else
		{
			std::cout << "\nERROR, wrong action\n" << std::endl;
			return;
		}
	}
}

//bool mix()
//{
//
//}

int main()
{
	std::cout << "SYSTEM INFO!\n";
	size_t X = 10;
	size_t Y = 10;
	gem **arr = arrgenerations(X, Y);

	init(arr, X, Y);
	logik(arr, X, Y);    //delete threes
	std::cout << "\n\n\n\n\n\n\n\n\n\nSTART!\n\n";
	dump(arr, X, Y);
	play(arr, X, Y);
	for (size_t i = 0; i < X; i++)
	{
		delete []arr[X];
	}
	delete[]arr;
}

