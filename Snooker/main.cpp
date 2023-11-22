#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

bool isColorAtPosition(const sf::Image& image, const sf::Vector2f& position, const sf::Color& color) {
    if (position.x >= 0 && position.x < image.getSize().x && position.y >= 0 && position.y < image.getSize().y) {
        return image.getPixel(static_cast<unsigned int>(position.x), static_cast<unsigned int>(position.y)) == color;
    }
    return false;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1500, 750), "SFML Texture Drawing");

    sf::Texture texture;
    if (!texture.loadFromFile("textures/Table.png")) {
        // Handle error if the image fails to load
        return EXIT_FAILURE;
    }
    sf::Image textureTable;
    if (!texture.loadFromFile("textures/Table.png")) {
        // Handle error if the image fails to load
        return EXIT_FAILURE;
    }

    sf::Sprite sprite;
    sprite.setTexture(texture);

    sf::CircleShape WhiteBall(20); // Create a WhiteBall with a radius of 15 pixels
    WhiteBall.setFillColor(sf::Color::White); // Set color of the WhiteBall
    WhiteBall.setPosition(450, 355); // Example position for the center of the window

    sf::Vector2f WhiteBallVelocity(0, 0); // Initial velocity set to (0, 0)
    const float acceleration = 5.f;
    const float friction = 0.001f; // Friction factor for reducing velocity
    bool isMoving = false;
    sf::Vector2f startPos;

    sf::Clock clock; // SFML clock to control the update rate
    float deltaTime = 0.0f;


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                isMoving = true;
                startPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                if (isMoving) {
                    sf::Vector2f endPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    sf::Vector2f direction = startPos - endPos;
                    float power = std::min(10.0f, std::sqrt(direction.x * direction.x + direction.y * direction.y));

                    // Apply force to the white WhiteBall based on 'direction' and 'power'
                    // Adjust WhiteBall velocity or add force here
                    WhiteBallVelocity = direction * power * 0.1f; // Adjust multiplier for suitable speed

                    isMoving = false; // Reset the movement flag
                }
            }

        }

          // Calculate deltaTime to update at a fixed frame rate
        deltaTime += clock.restart().asSeconds();

        // Update ball position based on deltaTime
        while (deltaTime >= 1.0f / 30.0f) { // Update at 30fps
            // Update ball's velocity or apply forces here if needed
            WhiteBall.move(WhiteBallVelocity*deltaTime*acceleration);
            deltaTime -= 1.0f / 30.0f;
        }
        WhiteBallVelocity = WhiteBallVelocity - WhiteBallVelocity*friction; // Apply friction to reduce velocity gradually
         // If the velocity is very small, stop the ball completely
        if (std::abs(WhiteBallVelocity.x) < 0.001f && std::abs(WhiteBallVelocity.y) < 0.001f) {
            WhiteBallVelocity = sf::Vector2f(0.0f, 0.0f);
        }


        // Collision detection based on color
        sf::Vector2f ballPosition = WhiteBall.getPosition();
        sf::Color collisionColor(99, 220, 99);

        if (isColorAtPosition(textureTable, ballPosition, collisionColor)) {
            std::cout << "Collision detected at position: (" << ballPosition.x << ", " << ballPosition.y << ")" << std::endl;
            // Handle collision with the specified color here
            // Example: Invert the ball's velocity for a bounce effect
            WhiteBallVelocity.x *= -1;
            WhiteBallVelocity.y *= -1;
        }
        sf::Color pixelColor = textureTable.getPixel(static_cast<unsigned int>(ballPosition.x), static_cast<unsigned int>(ballPosition.y));
        std::cout << "Pixel color at position: R=" << static_cast<int>(pixelColor.r) << " G=" << static_cast<int>(pixelColor.g) << " B=" << static_cast<int>(pixelColor.b) << std::endl;


        window.clear();
        window.draw(sprite);
        window.draw(WhiteBall);

        window.display(); // Display the drawn sprite
    }

    return EXIT_SUCCESS;
}
