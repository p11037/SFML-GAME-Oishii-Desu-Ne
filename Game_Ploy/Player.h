#pragma once
#include <SFML\Graphics.hpp>
#include "Animation.h"
#include "Collider.h"
class Player
{
public:
	Player(sf::Texture* texture,sf::Vector2u imageCount,float switchTime,float speed ,float jumpHight);
	~Player();
	
	void Update(float deltaTime,float sp);
	void Draw(sf::RenderWindow& window);
	void OnCollosion(sf::Vector2f direction);

	sf::Vector2f GetPosition() {return body.getPosition();}
	Collider GetCollider() { return Collider(body); }
	bool canJump;
	sf::RectangleShape body;
private:
	
	Animation animation;
	unsigned int row,col;
	float speed;

	sf::Vector2f velocity;
	
	float jumpHight;
};

