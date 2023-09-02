#ifndef INPUT_H
#define INPUT_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../Controller/Player.h"
#include "../../engine.h"
#include <cmath>

class Engine;
class Player;

class Input
{
    public:
        Input(Engine& lEngine);

        void manageInputs(Player *player);

    private:
        Engine& engine;
        sf::Window* window;
        sf::Vector2i windowCenter;
        sf::Vector2i prevMousePosition;

        float playerRotation = 0.0f;
        int currentFrame = 0;
        const int resetFrequency = 5;
};

#endif