#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "ComplexPlane.h"
 
int main()
{
    int width = VideoMode::getDesktopMode().width / 2;
    int height = VideoMode::getDesktopMode().height / 2;
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML window");
    window.setFramerateLimit(60);

    ComplexPlane complexPlane(width, height);
    cout << width << " " << height << endl; // For debugging
 
    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("font/SFCamera.ttf"))
        return EXIT_FAILURE;
    sf::Text text("hi", font, 20);
 
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::MouseButtonPressed)
            {
                Vector2i mousePixel(event.mouseButton.x, event.mouseButton.y);

                // Right click -- Zoom out
                if (event.mouseButton.button == Mouse::Right)
                {
                    complexPlane.setCenter(mousePixel);
                    complexPlane.zoomOut();
                }
                // Left click -- Zoom in
                if (event.mouseButton.button == Mouse::Left)
                {
                    complexPlane.setCenter(mousePixel);
                    complexPlane.zoomIn();
                }
            }
            // Update when mouse is moved
            if (event.type == Event::MouseMoved)
            {
                Vector2i mousePixel(event.mouseMove.x, event.mouseMove.y);
                complexPlane.setMouseLocation(mousePixel);
            }
        }
        // Check if esc is pressed
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();

        complexPlane.updateRender();
        complexPlane.loadText(text);

        // Clear screen
        window.clear();
        complexPlane.draw(window, RenderStates::Default);
 
        // Draw the string
        window.draw(text);        
 
        // Update the window
        window.display();
    }
 
    return EXIT_SUCCESS;
}
