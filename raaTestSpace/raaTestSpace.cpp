// raaTestSpace.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <conio.h>

int main()
{
	char c = 5;
	char cc = 2;

	std::cout << (int)((c & 1 << 0) ? 1 : 0) << std::endl;
	std::cout << (int)((c & 1 << 1) ? 1 : 0) << std::endl;
	std::cout << (int)((c & 1 << 2) ? 1 : 0) << std::endl;

	c = c|cc;

	std::cout << std::endl << (int)((c & 1 << 0) ? 1 : 0) << std::endl;
	std::cout << (int)((c & 1 << 1) ? 1 : 0) << std::endl;
	std::cout << (int)((c & 1 << 2) ? 1 : 0) << std::endl;

    return _getch();
}

