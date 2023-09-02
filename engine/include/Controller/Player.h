#ifndef PLAYER_H
#define PLAYER_H

#include "../Renderer/LevelBuild.h"

#include "../../engine.h"

class Engine;

class Player
{
    public:

        Player(Engine& lEngine, player_info_t* playerInfo);

        int getNumber();

        float getPosX();

        float getPosY();

        float getAngle();

        void setPosX(float value);

        void setPosY(float value);

        void setAngle(float value);

    private:
        int number; 
        float posX;
        float posY;
        float angle;
        float height;

        Engine& engine;
};

#endif