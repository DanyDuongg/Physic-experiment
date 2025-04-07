#define NOMINMAX
#include <windows.h>
#include <iostream>
#include "Game.h"



int main()
{
	float mass_of_A;
	float mass_of_B;
	float speed_of_A;
	float speed_of_B;
	float friction_of_road;
	
	std::cout << "Enter the mass of object A: \n";
	std::cin >> mass_of_A;
	std::cout << "Enter the mass of object B: \n";
	std::cin >> mass_of_B;
	std::cout << "Enter the velocity of object A: \n";
	std::cin >> speed_of_A;
	std::cout << "Enter the velocity of object B: \n";
	std::cin >> speed_of_B;
	std::cout << "Enter the friction of the road(Mu in Greece): \n";
	std::cin >> friction_of_road;
    // Accleration feature
	// friction feature
	Game game(mass_of_A,mass_of_B,speed_of_A,speed_of_B,friction_of_road);

	while (game.isRunning())
	{
		game.update();
		game.render();
		Sleep(1);

	}
	return 0;
}