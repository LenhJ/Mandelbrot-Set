// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <complex>
#include "ComplexPlane.h"

// Namespaces
using namespace sf;
using namespace std;

int main()
{

	// Video mode and window
	int pixelWidth = VideoMode::getDesktopMode().width / 2;
	int pixelHeight = VideoMode::getDesktopMode().height / 2;

	VideoMode vm(pixelWidth, pixelHeight);

	RenderWindow window(vm, "Mandelbrot", Style::Default);

	ComplexPlane complexPlane(pixelWidth, pixelHeight);

	// Font and Text
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "Error loading font!" << std::endl;
	}

	sf::Text text;
	text.setFont(font);
	text.setString("Blank");
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);

	// Main loop
	while (window.isOpen())

	{
		/*
		****************************************
		INPUT
		****************************************
		*/

		Event event;

		// Quit the program when 'ESC' is pressed
		if (Keyboard::isKeyPressed(Keyboard::Escape))

		{
			
			window.close();
		}

		while (window.pollEvent(event))
		{ 
			// Quit the program when window is closed
			if (event.type == Event::Closed)
			{
				window.close();
			}

			// Zoom in/out
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					complexPlane.zoomIn();
					complexPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					complexPlane.zoomOut();
					complexPlane.setCenter({event.mouseButton.x, event.mouseButton.y});
				}
			}

			// Mouse location
			if (event.type == sf::Event::MouseMoved)
			{
				complexPlane.setMouseLocation({event.mouseMove.x, event.mouseMove.y});
			}

		}

		/*
		****************************************
		UPDATE
		****************************************
		*/

		complexPlane.updateRender();
		complexPlane.loadText(text);

		/*
		****************************************
		DRAW
		****************************************
		*/

		window.clear();

		window.draw(complexPlane);
		window.draw(text);

		window.display();

	}
}