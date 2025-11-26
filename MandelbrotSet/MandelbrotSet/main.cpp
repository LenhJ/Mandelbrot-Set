#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"
#include <iostream>

using namespace sf;
using namespace std;

int main()
{
    // Get desktop resolution and reduce for performance
    int width = VideoMode::getDesktopMode().width / 2;
    int height = VideoMode::getDesktopMode().height / 2;

    VideoMode vm(width, height);
    RenderWindow window(vm, "Mandelbrot Set", Style::Default);

    // Create ComplexPlane object
    ComplexPlane complexPlane(width, height);

    // Create font and text
    Font font;
    Text text;

    // Try to load common system fonts
    const char* fontPaths[] = {
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/tahoma.ttf",
        "C:/Windows/Fonts/times.ttf",
        "arial.ttf"
    };

    bool fontLoaded = false;
    for (const char* fontPath : fontPaths)
    {
        if (font.loadFromFile(fontPath))
        {
            fontLoaded = true;
            break;
        }
    }

    // If no font loaded, create a simple font-free version
    if (!fontLoaded)
    {
        cout << "Warning: Could not load any font. Text will not display." << endl;
        // Continue without font - the Mandelbrot will still work
    }
    else
    {
        text.setFont(font);
    }

    text.setCharacterSize(20);
    text.setFillColor(Color::White);
    text.setPosition(10, 10);

    // Main loop
    while (window.isOpen())
    {
        // Handle Input segment
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }

            if (event.type == Event::MouseButtonPressed)
            {
                Vector2i mousePos = Mouse::getPosition(window);
                if (event.mouseButton.button == Mouse::Right)
                {
                    complexPlane.zoomOut();
                    complexPlane.setCenter(mousePos);
                }
                else if (event.mouseButton.button == Mouse::Left)
                {
                    complexPlane.zoomIn();
                    complexPlane.setCenter(mousePos);
                }
            }

            if (event.type == Event::MouseMoved)
            {
                complexPlane.setMouseLocation(Mouse::getPosition(window));
            }
        }

        // Check for Escape key
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        // Update Scene segment
        complexPlane.updateRender();

        // Only load text if font was successfully loaded
        if (fontLoaded)
        {
            complexPlane.loadText(text);
        }

        // Draw Scene segment
        window.clear();
        complexPlane.draw(window, RenderStates::Default);

        // Only draw text if font was successfully loaded
        if (fontLoaded)
        {
            window.draw(text);
        }

        window.display();
    }

    return 0;
}