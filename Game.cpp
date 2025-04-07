#include "Game.h"

void Game::initVar()
{
	this->window = nullptr;
	this->timeMovingA = sf::Time::Zero;
	this->timeMovingB = sf::Time::Zero;
	this->isMovingA = false;
	this->isMovingB = false;
	this->gravity = 9.80665;
}

void Game::initWindow()
{
	this->videoMode.height = 500;
	this->videoMode.width = 600;
	this->window = new sf::RenderWindow(this->videoMode, "Physics experiment", sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
	this->window->setFramerateLimit(60);
}



void Game::initObject()
{
	float sizeOfA = mass_A*5;
	float sizeOfB = mass_B*5;
	float horizonLine = static_cast<float>(window->getSize().y) / 2;
	this->objA.setSize(sf::Vector2f(sizeOfA, sizeOfA));
	this->objB.setSize(sf::Vector2f(sizeOfB, sizeOfB));
	this->objB.setFillColor(sf::Color::Red); // right side
	this->objA.setFillColor(sf::Color::Blue); //left side
	this->objA.setPosition(0.f, horizonLine- sizeOfA);
	this->objB.setPosition(600.f - sizeOfB, horizonLine - sizeOfB);
	velocityA = sf::Vector2f(A_speed, 0);
	velocityB = sf::Vector2f(-B_speed, 0);
	
} 

void Game::initText()
{
	if (!gameFont.loadFromFile("font/Roboto-Regular.ttf"))
	{
		infoText.setFont(gameFont);
	}
	else
	{
		infoText.setFont(gameFont);
	}

	infoText.setCharacterSize(18);
	infoText.setFillColor(sf::Color::White);
	const float padding = 10.0f;
	infoText.setPosition(window->getSize().x - 160.f, padding);

}

void Game::updateInfoToText()
{
	float timeA = timeMovingA.asSeconds();
	float timeB = timeMovingB.asSeconds();
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2)
		<< "Collisions: " << num_of_collision << "\n"
		<< "A: " << std::abs(velocityA.x) << "m/s" << "(" << mass_B << "kg" << ")" << "\n"
		<< "B: " << std::abs(velocityB.x) << "m/s" << "(" << mass_A << "kg" << ")" << "\n"
		<< "Time Moving:\n"
		<< "A: " << timeA << "s (" << (isMovingA ? "MOVING" : "STOPPED") << ")\n"
		<< "B: " << timeB << "s (" << (isMovingB ? "MOVING" : "STOPPED") << ")\n";
		
	
	infoText.setString(ss.str());
	infoText.setPosition(window->getSize().x -160.f, 10.f);

}

Game::Game(float msA, float msB, float spA, float spB,float fric) : mass_A(msA), mass_B(msB), A_speed(spA), B_speed(spB),friction(fric)
{
	this->initVar();
	this->initWindow();
	this->initObject();
	this->initText();
}

Game::~Game()
{
	delete this->window;
}

// Accesstor implement here.
const bool Game::isRunning()
{
	return this->window->isOpen();
}



void Game::pollEvent()
{
	while (this->window->pollEvent(this->event))
	{
		switch (this->event.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		}
	}
}

void Game::handleFriction()
{
	sf::Time deltaTime = movementClock.restart();
	float deltaSeconds = deltaTime.asSeconds();
	if (friction != 0)
	{
		float deceleration = gravity * friction;
		velocityA.x= velocityA.x- ((velocityA.x > 0) ? deceleration : -deceleration)*deltaSeconds;
		velocityB.x= velocityB.x - ((velocityB.x > 0) ? deceleration : -deceleration) * deltaSeconds;
		if (std::abs(velocityA.x )<= 0.01f)
		{
			isMovingA = false;
		}
		if (std::abs(velocityB.x) <= 0.01f)
		{
			isMovingB = false;
		}

	}
	
}

void Game::handleCollision()
{
	if (!objA.getGlobalBounds().intersects(objB.getGlobalBounds()))
		return;

	num_of_collision += 1;
	// According to conservation of momentum
	float newVelocityA = ((mass_A - mass_B) * velocityA.x + 2 * velocityB.x * mass_B) / (mass_A + mass_B);
	float newVelocityB = ((mass_B - mass_A) * velocityB.x + 2 * velocityA.x * mass_A) / (mass_A + mass_B);
	velocityA.x = newVelocityA;
	velocityB.x = newVelocityB;

	float overlap = (objA.getGlobalBounds().left + objA.getGlobalBounds().width) - objB.getGlobalBounds().left;
	objA.move(-overlap / 2.f, 0.f);
	objB.move(overlap / 2.f, 0.f);

}

void Game::update()
{
	sf::Time deltaTime = movementClock.restart();
	if (std::abs(velocityA.x) > 0.01f)
	{
		timeMovingA += deltaTime;
		isMovingA = true;
	}
	else
	{
		velocityA.x = 0;
		isMovingA = false;
	}
		
	if (std::abs(velocityB.x) > 0.01f)
	{
		timeMovingB += deltaTime;
		isMovingB = true;
	}
	else
	{
		velocityB.x = 0;
		isMovingB = false;
	}






	this->pollEvent();
	this->updateInfoToText();
	this->objA.move(this->velocityA);
	this->objB.move(this->velocityB);
	handleFriction();
	handleCollision();


	if (objA.getPosition().x <= 0)
	{
		objA.setPosition(0, objA.getPosition().y);
		velocityA.x = -velocityA.x;
	}

	else if (objA.getPosition().x + objA.getSize().x >= static_cast<float>(window->getSize().x))
	{
		objA.setPosition(static_cast<float>(window->getSize().x) - objA.getSize().x, objA.getPosition().y);
		velocityA.x = -velocityA.x;
	}

	else if (objB.getPosition().x <= 0)
	{
		objB.setPosition(0, objB.getPosition().y);
		velocityB.x = -velocityB.x;
	}

	else if (objB.getPosition().x + objB.getSize().x >= static_cast<float>(window->getSize().x))
	{
		objB.setPosition(static_cast<float>(window->getSize().x) - objB.getSize().x, objB.getPosition().y);
		velocityB.x = -velocityB.x;
	}


}

void Game::render()
{
	this->window->clear();
	// draw the road
	sf::VertexArray horizon(sf::Lines, 2);
	float horizonLine1 = static_cast<float>(window->getSize().y) / 2;
	horizon[0].position = sf::Vector2f(0.0f, horizonLine1);
	horizon[1].position = sf::Vector2f(600, horizonLine1);
	horizon[0].color = sf::Color::Green;
	horizon[1].color = sf::Color::Green;
	// draw game here.
	this->window->draw(horizon);
	this->window->draw(objA);
	this->window->draw(objB);
	//TEXT
	this->window->draw(infoText);


	
	this->window->display();
}