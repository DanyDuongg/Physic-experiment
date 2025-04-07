#pragma once
#define NOMINMAX  
#include <SFML/Graphics.hpp>
#include <cmath>
#include <sstream>
#include <iomanip>

class Game
{
private:
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event event;

	sf::RectangleShape objA;
	sf::RectangleShape objB;
	int num_of_collision;
	float mass_A;
	float mass_B;
	float A_speed;
	float B_speed;
	sf::Vector2f velocityA;
	sf::Vector2f velocityB;
	float gravity;
	float friction;
	sf::Text infoText;
	sf::Font gameFont;
	//Time
	sf::Clock movementClock;
	sf::Time timeMovingA;
	sf::Time timeMovingB;
	bool isMovingA;
	bool isMovingB;
	


	void initVar();
	void initWindow();
	void initObject();
	void initText();
	void updateInfoToText();


public:
	Game(float msA,float msB, float spA, float spB,float fric);
	virtual ~Game();
	const bool isRunning();
	void pollEvent();
	void handleFriction();
	void handleCollision();
	void update();
	void render();

};