#include <SFML/Graphics.hpp>
#include "include/GameConfig.h"
#include "game.cpp"  

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Centipede");
    Game game(window);

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            game.handleEvent(event);
        }

        game.update();
        game.draw();

        sf::sleep(sf::milliseconds(16));  // Simple FPS control
    }

    return 0;
}
