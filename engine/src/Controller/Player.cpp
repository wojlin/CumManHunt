#include "../../include/Controller/Player.h"

Player::Player(Engine& lEngine, player_info_t* playerInfo): engine(lEngine)
{
    number = playerInfo->number;
    posX = playerInfo->posX;
    posY = playerInfo->posY;
    angle = playerInfo->angle;
    height = *(engine.getPlayerHeight());
}

int Player::getNumber()
{
    return number;
}

float Player::getPosX()
{
    return posX;
}

float Player::getPosY()
{
    return posY;
}

float Player::getAngle()
{
    return angle;
}

float Player::getHeight()
{
    return height;
}


void Player::setPosX(float value)
{
    posX = value;
}

void Player::setPosY(float value)
{
    posY = value;
}

void Player::setAngle(float value)
{
    angle = value;
}

void Player::setHeight(float value)
{
    height = value;
}