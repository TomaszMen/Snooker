#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

bool checkCollision(sf::CircleShape& ball1, sf::CircleShape& ball2) {
    sf::Vector2f delta = ball1.getPosition() - ball2.getPosition();
    float distanceSquared = delta.x * delta.x + delta.y * delta.y;
    float radiusSquared = (ball1.getRadius() + ball2.getRadius()) * (ball1.getRadius() + ball2.getRadius());
    return distanceSquared <= radiusSquared;
}

sf::Vector2f getNormalVector(const sf::Vector2f& p1, const sf::Vector2f& p2) {
    sf::Vector2f normal = p2 - p1;
    float length = sqrt(normal.x * normal.x + normal.y * normal.y);
    return sf::Vector2f(-normal.y / length, normal.x / length); // Normalizacja wektora
}

// Funkcja do aktualizacji prędkości po kolizji zgodnie z fizyką
void handleCollision(sf::CircleShape& ball1, sf::CircleShape& ball2) {
    sf::Vector2f vel1 = ball1.getPosition() - ball2.getPosition();
    sf::Vector2f vel2 = ball2.getPosition() - ball1.getPosition();
    float distance = sqrt(vel1.x * vel1.x + vel1.y * vel1.y);
    vel1 /= distance;
    vel2 /= distance;

    float dot1 = vel1.x * ball1.getRadius() + vel1.y * ball1.getRadius();
    float dot2 = vel2.x * ball2.getRadius() + vel2.y * ball2.getRadius();

    sf::Vector2f newVel1 = vel1 * dot1 + vel2 * dot2;
    sf::Vector2f newVel2 = vel1 * dot2 + vel2 * dot1;

    ball1.setPosition(ball2.getPosition() + newVel1);
    ball2.setPosition(ball1.getPosition() + newVel2);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1500, 750), "Snooker");

    sf::Texture table;
    if (!table.loadFromFile("textures/Table.png")) {
        return 0;
    }
    sf::Texture coll_table;
    if(!coll_table.loadFromFile("textures/collision_table.png")){
        return 0;
    }
    // Tworzenie wektora bil
    std::vector<sf::CircleShape> bilardoweKule;

    // Dodawanie 15 czerwonych kul
    for (int i = 0; i < 15; ++i) {
        sf::CircleShape kula(16);
        kula.setFillColor(sf::Color (255, 0, 0));
        bilardoweKule.push_back(kula);
    }

    // Dodawanie pozosta³ych kul
    sf::CircleShape kula(16);
    kula.setFillColor(sf::Color (0, 0, 0));
    bilardoweKule.push_back(kula);

    kula.setFillColor(sf::Color (0, 0, 255));
    bilardoweKule.push_back(kula);

    kula.setFillColor(sf::Color (165, 42, 42));
    bilardoweKule.push_back(kula);

    kula.setFillColor(sf::Color (255, 192, 203));
    bilardoweKule.push_back(kula);

    kula.setFillColor(sf::Color (255, 255, 0));
    bilardoweKule.push_back(kula);

    kula.setFillColor(sf::Color (0, 128, 0));
    bilardoweKule.push_back(kula);

    //Ustawianie pozycji bil
    int x = 966;
    int y = 367;
    int limit = 1;
    int licznik = 0;
    for (int i = 0; i < 15; i++) {
        bilardoweKule[i].setPosition(x, y);
        licznik++;
        std::cout<<x<<"  "<<y<<std::endl;
        if (licznik == limit){
            x+=29;
            y-=((17+(limit-1)*34));
            licznik = 0;
            limit++;
        } else {
            y+=34;
        }
    }
    bilardoweKule[15].setPosition(x+40,367);
    bilardoweKule[16].setPosition(742,367);
    bilardoweKule[17].setPosition(500,367);
    bilardoweKule[18].setPosition(932,367);
    bilardoweKule[19].setPosition(500,447);
    bilardoweKule[20].setPosition(500,287);
    bilardoweKule[21].setPosition(500,500);

    sf::Sprite sprite;
    sprite.setTexture(table);

    sf::CircleShape WhiteBall(16);
    WhiteBall.setFillColor(sf::Color::White);
    WhiteBall.setPosition(460, 410);

    sf::Vector2f WhiteBallVelocity(0, 0);
    const float acceleration = 5.f;
    const float friction = 0.001f;
    bool isMoving = false;
    sf::Vector2f startPos;

    sf::Clock clock;
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

                    WhiteBallVelocity = direction * power * 0.1f;

                    isMoving = false;
                }
            }

        }
        for (size_t i = 0; i < bilardoweKule.size(); ++i) {
            if (&WhiteBall != &bilardoweKule[i] && checkCollision(WhiteBall, bilardoweKule[i])) {
                handleCollision(WhiteBall, bilardoweKule[i]);
            }
        }
        deltaTime += clock.restart().asSeconds();
        while (deltaTime >= 1.0f / 30.0f) {
            WhiteBall.move(WhiteBallVelocity*deltaTime*acceleration);
            deltaTime -= 1.0f / 30.0f;
        }
        WhiteBallVelocity = WhiteBallVelocity - WhiteBallVelocity*friction;
        if (std::abs(WhiteBallVelocity.x) < 0.001f && std::abs(WhiteBallVelocity.y) < 0.001f) {
            WhiteBallVelocity = sf::Vector2f(0.0f, 0.0f);
        }

        window.clear();
        window.draw(sprite);
        for (auto& k : bilardoweKule) {
            window.draw(k);
        }
        window.draw(WhiteBall);

        window.display(); // Display the drawn sprite
    }

    return 0;
}
