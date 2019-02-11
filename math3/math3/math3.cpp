//#include "pch.h"
#include <time.h>
#include <iostream>
#include <string>
using namespace std;

struct gem;
gem **arrgenerations(size_t X, size_t Y);
void create_random_color(gem &element);
void init(gem **Array, size_t X, size_t Y);
void tick(gem **Array, size_t X, size_t Y, int &A);
void step(gem **Array, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, size_t X, size_t Y);
void move(gem **Array, unsigned int x1, unsigned int y1, char m, size_t X, size_t Y);
void offset_crystal_removal(gem **Array, size_t X, size_t Y, unsigned int sum);
bool search_thee(gem **Array, size_t X, size_t Y);
void delete_three(gem **Array, size_t X, size_t Y);
void dump(gem **Array, size_t X, size_t Y);
void play(gem **Array, size_t X, size_t Y);
bool check(gem **Array, size_t X, size_t Y);
bool check_move(gem **Array, unsigned int x1, unsigned int y1, char m, size_t X, size_t Y);
void mix(gem **Array, size_t X, size_t Y);

int actions = 0;



struct gem {
	char color = '*';
	//char special = NULL;                   //usless
	bool destroy = false;
};

gem **arrgenerations(size_t X, size_t Y) {
	gem **newarr = new gem*[X];
	for (size_t i = 0; i < X; i++) {
		newarr[i] = new gem[Y];
	}
	return newarr;
}

//char create_random_special() {                    //usless
//	if ((rand() % 10)==1) {
//		return (char)(rand() % 6 + 65);
//	}
//}

void create_random_color(gem &element) {
	element.color = (char)(rand() % 6 + 65);
	element.destroy = false;
}

void init(gem **Array, size_t X, size_t Y) {
	for (size_t i = 0; i < X; i++) {
		for (size_t j = 0; j < Y; j++) {
			create_random_color(Array[i][j]);
		}
	}
    delete_three(Array, X, Y);
}

void tick(gem **Array, size_t X, size_t Y, int &A) {
	dump(Array, X, Y);
	A++;
}

void step(gem **Array, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, size_t X, size_t Y) {
    swap(Array[x1][y1], Array[x2][y2]);
    if (!search_thee(Array, X, Y)) {
        swap(Array[x1][y1], Array[x2][y2]);
        std::cout << "Unsuitable combination" << std::endl;
    } else {
        tick(Array, X, Y, actions);
    }
}

void move(gem **Array, unsigned int x1, unsigned int y1, char m, size_t X, size_t Y) {
    if (10 > x1 && x1 >= 0 && 10 > y1 && y1 >= 0 && ((m == 'l' && y1 != 0) ||
        (m == 'r' && y1 != 9) || (m == 'u'&& x1 != 0) || (m == 'd'&& x1 != 9))) {
        switch (m) {
            case 'l':
                step(Array, x1, y1, x1, y1 - 1, X, Y);
                break;
            case 'r':
                step(Array, x1, y1, x1, y1 + 1, X, Y);
                break;
            case 'u':
                step(Array, x1, y1, x1 - 1, y1, X, Y);
                break;
            case 'd':
                step(Array, x1, y1, x1 + 1, y1, X, Y);
                break;
            default:
                std::cout << "Unsuitable combination" << std::endl;
                return;
                break;
        }
    } else {
        std::cout << "\nERROR wrong command call, see definition 'move'\n" << std::endl;
        return;
    }
}

void offset_crystal_removal(gem **Array, size_t X, size_t Y, unsigned int sum) {
	int h = 0;
	while (h!=sum) {
		for (size_t i = 0; i < X; i++) {
			for (size_t j = 0; j < Y; j++) {
				if (Array[i][j].destroy) {
					if (i==0) {
						create_random_color(Array[i][j]);
						//action = tick(Array, X, Y, action);
						h++;
					} else {
						swap(Array[i][j], Array[i - 1][j]);
						//action = tick(Array, X, Y, action);
						//std::cout << "\nkristal move for delete "<<i<<" "<<j<<" in "<< i-1 <<" "<<j<<std::endl;
					}					
				}
			}
		}
	}
}

bool search_thee(gem **Array, size_t X, size_t Y) {
	bool flag = false;
	for (size_t i = 0; i < X; i++) {        // search lines	
		for (size_t j = 0; j < Y-2; j++) {
			if (Array[i][j].color == Array[i][j + 1].color && 
                Array[i][j + 1].color == Array[i][j + 2].color) {
				flag = true;
				for (size_t k = j + 3; k < Y; k++) {
					if (Array[i][j].color == Array[i][k].color) {
						std::cout << "\nkristal destroy " << i << " " << j<<std::endl;
						Array[i][k].destroy = true;
					}
					else {
						break;
					}
				}
				Array[i][j].destroy = Array[i][j + 1].destroy = Array[i][j + 2].destroy = true;
				std::cout << "\nkristal destroy " << i << " " << j <<" : " << i << " " << j+1 << " : " << i << " " << j+2 << std::endl;
			}
		}
	}
	for (size_t j = 0; j < Y; j++) {        // search columns
		for (size_t i = 0; i < X - 2; i++) {
			if (Array[i][j].color == Array[i + 1][j].color &&
                Array[i + 1][j].color == Array[i + 2][j].color) {
				flag = true;
				for (size_t k = i + 3; k < X; k++) {
					if (Array[i][j].color == Array[k][j].color) {
						Array[k][j].destroy = true;
						std::cout << "\nkristal destroy " << i << " " << j << std::endl;
					} else {
						break;
					}
				}
				Array[i][j].destroy = Array[i + 1][j].destroy = Array[i + 2][j].destroy = true;
				std::cout << "\nkristal destroy " << i << " " << j << " : " << i+1 << " " << j  << " : " << i+2 << " " << j << std::endl;
			}
		}
	}
	if (flag) {
		int sum = 0;
		for (size_t i = 0; i < X; i++) {
			for (size_t j = 0; j < Y; j++) {
				if (Array[i][j].destroy) {
					sum++;
				}
			}
		}
		offset_crystal_removal(Array, X, Y, sum);
		return true;
	} else {
		return false;
	}
}

void delete_three(gem **Array, size_t X, size_t Y) {
	if (search_thee(Array, X, Y)) {
        delete_three(Array, X, Y);
	}
}

void dump(gem **Array, size_t X, size_t Y) {
	std::cout << " y0123456789\nx|----------\n";
	for (size_t i = 0; i < X; i++) {
		std::cout << i<<"|";
		for (size_t j = 0; j < Y; j++) {
			std::cout << Array[i][j].color;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void play(gem **Array, size_t X, size_t Y) {
    string command = "";
	char m = '\0';
	int x1 = -1;
	int y1 = -1;
    std::cout << "Have a nice game\nEnter 'h' to Help\n";
	while (true) {
		std::cin >> command;
		if (command[0] =='m' && command.length() == 4) {
            x1 = command[1]-'0';
            y1 = command[2]-'0';
            m = command[3];
			for (int i = 0; i < 25; i++) {
				std::cout << std::endl;
			}
			move(Array, x1, y1, m, X, Y);
		}	
		else if (command[0] == 'q' && command.length() == 1) {
			std::cout << "Exit, good luck\n";
			return;
        } else if (command[0]=='h' && command.length() == 1) {
            std::cout << " Enter 'q' to exit game\nexample input for move - m x y d \n x, у - coordinates(0 - 9)\nd - movement in(lrud - left, right, up, down)\n\n";
        } else {
			std::cout << "\nERROR, wrong action\n" << std::endl;
		}
        mix(Array, X, Y);
	}
}


bool check(gem **Array, size_t X, size_t Y) {
    for (size_t i = 0; i < X; i++) {        // search lines	
        for (size_t j = 0; j < Y - 2; j++) {
            if (Array[i][j].color == Array[i][j + 1].color &&
                Array[i][j + 1].color == Array[i][j + 2].color) {
                return true;
            }
        }
    }
    for (size_t j = 0; j < Y; j++) {        // search columns
        for (size_t i = 0; i < X - 2; i++) {
            if (Array[i][j].color == Array[i + 1][j].color &&
                Array[i + 1][j].color == Array[i + 2][j].color) {
                return true;
            }
        }
    }
    return false;
}

bool check_move(gem **Array, unsigned int x1, unsigned int y1, char m, size_t X, size_t Y) {
    if (10 > x1 && x1 >= 0 && 10 > y1 && y1 >= 0 && ((m == 'l' && y1 != 0) ||
        (m == 'r' && y1 != 9) || (m == 'u'&& x1 != 0) || (m == 'd'&& x1 != 9))) {
        switch (m) {
            case 'l':
                swap(Array[x1][y1], Array[x1][y1 - 1]);
                if (check(Array, X, Y)) {
                    return true;
                }
                swap(Array[x1][y1], Array[x1][y1 - 1]);
                break;
            case 'r':
                swap(Array[x1][y1], Array[x1][y1 + 1]);
                if (check(Array, X, Y)) {
                    return true;
                }
                swap(Array[x1][y1], Array[x1][y1 + 1]);
                break;
            case 'u':
                swap(Array[x1][y1], Array[x1 - 1][y1]);
                if (check(Array, X, Y)) {
                    return true;
                }
                swap(Array[x1][y1], Array[x1 - 1][y1]);
                break;
            case 'd':
                swap(Array[x1][y1], Array[x1 + 1][y1]);
                if (check(Array, X, Y)) {
                    return true;
                }
                swap(Array[x1][y1], Array[x1 + 1][y1]);
                break;
            default:
                break;
        }
    }
    return false;
}

void mix(gem **Array, size_t X, size_t Y) {
    for (size_t i = 0; i < X; i++) {
        for (size_t j = 0; j < Y; j++) {
            if (check_move(Array, i, j, 'l', X, Y)) {
                return;
            } else if (check_move(Array, i, j, 'r', X, Y)) {
                return;
            } else if (check_move(Array, i, j, 'u', X, Y)) {
                return;
            } else if (check_move(Array, i, j, 'd', X, Y)) {
                return;
            }
        }
    }
    std::cout << "\nThere are no possible moves, the field is updated.\n";
    init(Array, X, Y);
	return;
}

int main() {
	std::cout << "SYSTEM INFO!\n";
	size_t X = 10;
	size_t Y = 10;
	gem **arr = arrgenerations(X, Y);

	init(arr, X, Y);
    delete_three(arr, X, Y);    //delete threes
	std::cout << "\n\n\n\n\n\n\n\n\n\nSTART!\n\n";
	dump(arr, X, Y);
	play(arr, X, Y);
	for (size_t i = 0; i < X; i++) {
		delete [] arr[i];
	}
    delete [] arr;
    cout << "t";
}

