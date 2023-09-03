
#include "../../include/Controller/Input.h"
#include "../../include/Controller/Player.h"

Input::Input(Engine& lEngine) : engine(lEngine)
{
    window = engine.getWindow();
    windowCenter = sf::Vector2i(window->getSize().x / 2, window->getSize().y / 2);
    sf::Mouse::setPosition(windowCenter, *window);
}

void Input::manageInputs(Player *player)
{

    sf::Event event;
    while (window->pollEvent(event))
    {     
        if (event.type == sf::Event::Closed)
        {
            window->close();
        }    
    }  

    // Calculate mouse movement
    sf::Vector2i currentMousePosition = sf::Mouse::getPosition(*window);
    sf::Vector2i mouseMovement = currentMousePosition - windowCenter;
    // Reset the mouse position to the center of the screen periodically
    if (currentFrame >= resetFrequency) {
        sf::Mouse::setPosition(windowCenter, *window);
        currentFrame = 0;
    } else {
        currentFrame++;
    }

    // Update player rotation based on mouse movement
    playerRotation -= static_cast<float>(mouseMovement.x) * *(engine.getPlayerRotationSpeed()) * *(engine.getDeltaSeconds());
    sf::Vector2f movementDirection(std::cos(playerRotation * M_PI / 180.0f), std::sin(playerRotation * M_PI / 180.0f));



    sf::Vector2f forward(std::cos(playerRotation * (3.14159f / 180.0f)), std::sin(playerRotation * (3.14159f / 180.0f)));
    sf::Vector2f movement(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        movement += forward;
    }   
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        movement -= forward;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        movement -= sf::Vector2f(-forward.y, forward.x);
    }          
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
            movement += sf::Vector2f(-forward.y, forward.x);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        cout << "close" << endl;
        window->close();
    }

    if (movement != sf::Vector2f(0.0f, 0.0f)) 
    {
        float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        movement /= length;
        player->setPosX(player->getPosX() + *(engine.getPlayerSpeed()) * *(engine.getDeltaSeconds()) * movement.x);
        player->setPosY(player->getPosY() + *(engine.getPlayerSpeed()) * *(engine.getDeltaSeconds()) * movement.y);
    }
    
    player->setAngle(playerRotation);
}