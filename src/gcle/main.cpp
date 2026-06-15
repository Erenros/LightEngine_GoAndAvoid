
#include <SFML/Graphics.hpp>
#include <iostream>
 
int main()
{
	sf::Window* mpWindow = new sf::RenderWindow(sf::VideoMode(800, 600), "title");
	mpWindow->setFramerateLimit(60); 
}