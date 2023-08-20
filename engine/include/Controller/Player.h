#ifndef PLAYER_H
#define PLAYER_H

#include "../Renderer/LevelBuild.h"

class Player
{
    public:

        Player(LevelBuild::player_info_t* playerInfo)
        {
            number = playerInfo->number;
            posX = playerInfo->posX;
            posY = playerInfo->posY;
            angle = playerInfo->angle;
        }

        int getNumber()
        {
            return number;
        }

        float getPosX()
        {
            return posX;
        }

        float getPosY()
        {
            return posY;
        }

        float getAngle()
        {
            return angle;
        }


        void setPosX(float value)
        {
            posX = value;
        }

        void setPosY(float value)
        {
            posY = value;
        }

        void setAngle(float value)
        {
            angle = value;
        }

    private:
        int number; 
        float posX;
        float posY;
        float angle;
};

#endif