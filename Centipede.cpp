#include "include/GameConfig.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

class Game {
private:
    sf::RenderWindow& window;
    GameState currentState;

    // Textures
    sf::Texture backgroundTexture;
    sf::Texture bulletTexture;
    sf::Texture headTexture;
    sf::Texture bodyTexture;
    sf::Texture mushroomTexture;
    sf::Texture shipTexture;

    // Sprites
    sf::Sprite background;
    sf::Sprite ship;
    std::vector<sf::Sprite> mushrooms;
    std::vector<sf::Sprite> centipede;
    std::vector<sf::Sprite> bullets;

    // UI Elements
    sf::Font font;
    sf::Text titleText;
    sf::Text playText;
    sf::Text quitText;
    sf::Text gameOverText;
    sf::Text restartText;

    // Game variables
    int score;
    float bulletSpeed;
    float centipedeSpeed;
    std::vector<int> mushroomHealth;

public:
    Game(sf::RenderWindow& win) : window(win), currentState(GameState::MENU) {
        srand(time(0));
        loadTextures();
        setupSprites();
        setupUI();
    }

    void loadTextures() {
        if (!backgroundTexture.loadFromFile("assets/background.png")) {
            // Handle error
        }
        if (!bulletTexture.loadFromFile("assets/bullet.png")) {
            // Handle error
        }
        if (!headTexture.loadFromFile("assets/head.png")) {
            // Handle error
        }
        if (!bodyTexture.loadFromFile("assets/body.png")) {
            // Handle error
        }
        if (!mushroomTexture.loadFromFile("assets/mushroom.png")) {
            // Handle error
        }
        if (!shipTexture.loadFromFile("assets/ship.png")) {
            // Handle error
        }
        if (!font.loadFromFile("assets/font.ttf")) {
            // Handle error (you'll need to provide a font file)
        }
    }

    void setupSprites() {
        // Background
        background.setTexture(backgroundTexture);
        background.setScale(
            WINDOW_WIDTH / background.getLocalBounds().width,
            WINDOW_HEIGHT / background.getLocalBounds().height
        );

        // Ship
        ship.setTexture(shipTexture);
        ship.setPosition(WINDOW_WIDTH / 2 - ship.getLocalBounds().width / 2,
            WINDOW_HEIGHT - ship.getLocalBounds().height);

        // Initialize game variables
        score = 0;
        bulletSpeed = 500.0f;
        centipedeSpeed = 100.0f;
    }

    void setupUI() {
        titleText.setFont(font);
        titleText.setString("CENTIPEDE");
        titleText.setCharacterSize(64);
        titleText.setFillColor(sf::Color::Green);
        titleText.setPosition(WINDOW_WIDTH / 2 - titleText.getLocalBounds().width / 2, 100);

        playText.setFont(font);
        playText.setString("PLAY");
        playText.setCharacterSize(48);
        playText.setFillColor(sf::Color::White);
        playText.setPosition(WINDOW_WIDTH / 2 - playText.getLocalBounds().width / 2, 300);

        quitText.setFont(font);
        quitText.setString("QUIT");
        quitText.setCharacterSize(48);
        quitText.setFillColor(sf::Color::White);
        quitText.setPosition(WINDOW_WIDTH / 2 - quitText.getLocalBounds().width / 2, 400);

        gameOverText.setFont(font);
        gameOverText.setString("GAME OVER");
        gameOverText.setCharacterSize(64);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(WINDOW_WIDTH / 2 - gameOverText.getLocalBounds().width / 2, 100);

        restartText.setFont(font);
        restartText.setString("PLAY AGAIN");
        restartText.setCharacterSize(48);
        restartText.setFillColor(sf::Color::White);
        restartText.setPosition(WINDOW_WIDTH / 2 - restartText.getLocalBounds().width / 2, 300);
    }

    void startGame() {
        currentState = GameState::PLAYING;

        // Clear existing game objects
        mushrooms.clear();
        centipede.clear();
        bullets.clear();
        mushroomHealth.clear();

        // Spawn mushrooms
        spawnMushrooms(20);

        // Spawn centipede
        spawnCentipede();

        // Reset ship position
        ship.setPosition(WINDOW_WIDTH / 2 - ship.getLocalBounds().width / 2,
            WINDOW_HEIGHT - ship.getLocalBounds().height);
    }

    void spawnMushrooms(int count) {
        for (int i = 0; i < count; i++) {
            sf::Sprite mushroom;
            mushroom.setTexture(mushroomTexture);

            // Random position that's aligned with the grid
            int x = (rand() % GRID_WIDTH) * TILE_SIZE;
            int y = (rand() % (GRID_HEIGHT - 5)) * TILE_SIZE; // Leave space at bottom

            mushroom.setPosition(x, y);
            mushrooms.push_back(mushroom);
            mushroomHealth.push_back(2); // Mushrooms take 2 hits to destroy
        }
    }

    void spawnCentipede() {
        // Clear existing centipede
        centipede.clear();

        // Create head
        sf::Sprite head;
        head.setTexture(headTexture);
        head.setPosition((rand() % (GRID_WIDTH - 10)) * TILE_SIZE, 0);
        centipede.push_back(head);

        // Create body segments
        for (int i = 1; i <= 10; i++) {
            sf::Sprite body;
            body.setTexture(bodyTexture);
            body.setPosition(head.getPosition().x - i * TILE_SIZE, 0);
            centipede.push_back(body);
        }
    }

    void handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (currentState == GameState::MENU) {
                if (playText.getGlobalBounds().contains(mousePos)) {
                    startGame();
                }
                else if (quitText.getGlobalBounds().contains(mousePos)) {
                    window.close();
                }
            }
            else if (currentState == GameState::GAME_OVER) {
                if (restartText.getGlobalBounds().contains(mousePos)) {
                    startGame();
                }
                else if (quitText.getGlobalBounds().contains(mousePos)) {
                    window.close();
                }
            }
        }

        if (currentState == GameState::PLAYING) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    // Shoot bullet
                    sf::Sprite bullet;
                    bullet.setTexture(bulletTexture);
                    bullet.setPosition(
                        ship.getPosition().x + ship.getLocalBounds().width / 2 - bullet.getLocalBounds().width / 2,
                        ship.getPosition().y
                    );
                    bullets.push_back(bullet);
                }
            }
        }
    }

    void update() {
        if (currentState != GameState::PLAYING) return;

        // Move ship with arrow keys
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && ship.getPosition().x > 0) {
            ship.move(-5, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
            ship.getPosition().x < WINDOW_WIDTH - ship.getLocalBounds().width) {
            ship.move(5, 0);
        }

        // Update bullets
        for (auto it = bullets.begin(); it != bullets.end();) {
            it->move(0, -bulletSpeed * 0.016f); // Assuming 60 FPS

            // Remove bullets that go off screen
            if (it->getPosition().y < 0) {
                it = bullets.erase(it);
            }
            else {
                // Check bullet collisions with mushrooms
                bool bulletHit = false;
                for (size_t i = 0; i < mushrooms.size(); i++) {
                    if (it->getGlobalBounds().intersects(mushrooms[i].getGlobalBounds())) {
                        mushroomHealth[i]--;
                        if (mushroomHealth[i] <= 0) {
                            mushrooms.erase(mushrooms.begin() + i);
                            mushroomHealth.erase(mushroomHealth.begin() + i);
                        }
                        bulletHit = true;
                        break;
                    }
                }

                // Check bullet collisions with centipede
                if (!bulletHit && !centipede.empty()) {
                    for (auto centIt = centipede.begin(); centIt != centipede.end();) {
                        if (it->getGlobalBounds().intersects(centIt->getGlobalBounds())) {
                            it = bullets.erase(it);
                            centIt = centipede.erase(centIt);
                            bulletHit = true;
                            break;
                        }
                        else {
                            ++centIt;
                        }
                    }
                }

                if (!bulletHit) {
                    ++it;
                }
            }
        }

        // Update centipede movement
        if (!centipede.empty()) {
            // Simple movement - move right until hitting edge or mushroom
            centipede[0].move(centipedeSpeed * 0.016f, 0);

            // Check for collision with right edge or mushroom
            bool hitObstacle = false;
            if (centipede[0].getPosition().x >= WINDOW_WIDTH - TILE_SIZE) {
                hitObstacle = true;
            }
            else {
                for (const auto& mushroom : mushrooms) {
                    if (centipede[0].getGlobalBounds().intersects(mushroom.getGlobalBounds())) {
                        hitObstacle = true;
                        break;
                    }
                }
            }

            if (hitObstacle) {
                // Move down and reverse direction
                centipede[0].move(0, TILE_SIZE);
                centipedeSpeed *= -1;

                // Check if reached bottom
                if (centipede[0].getPosition().y >= WINDOW_HEIGHT - TILE_SIZE) {
                    currentState = GameState::GAME_OVER;
                }
            }

            // Update body segments to follow head
            for (size_t i = 1; i < centipede.size(); i++) {
                sf::Vector2f targetPos = centipede[i - 1].getPosition();
                centipede[i].setPosition(targetPos);
            }
        }
    }

    void draw() {
        window.clear();

        if (currentState == GameState::MENU) {
            window.draw(background);
            window.draw(titleText);
            window.draw(playText);
            window.draw(quitText);
        }
        else if (currentState == GameState::PLAYING) {
            window.draw(background);

            // Draw mushrooms
            for (const auto& mushroom : mushrooms) {
                window.draw(mushroom);
            }

            // Draw centipede
            for (const auto& segment : centipede) {
                window.draw(segment);
            }

            // Draw bullets
            for (const auto& bullet : bullets) {
                window.draw(bullet);
            }

            // Draw ship
            window.draw(ship);
        }
        else if (currentState == GameState::GAME_OVER) {
            window.draw(background);
            window.draw(gameOverText);
            window.draw(restartText);
            window.draw(quitText);
        }
    }
};