#include "../../include/Renderer/LevelBuild.h"

LevelBuild::LevelBuild(LevelData::LevelData *lLevel, WADStructure::WADStructure *lWad)
{
    level = lLevel;
    wad = lWad;
    levelBounds = calculateLevelBounds();
    vertexsBounds = calculateVertexsBounds();
    playersInfo = calculatePlayersInfo();
}

level_bounds_t LevelBuild::getLevelBounds()
{
    return levelBounds;
}

vertexs_bounds_t LevelBuild::getVertexsBounds()
{
    return vertexsBounds;
}

vector<player_info_t>* LevelBuild::getPlayersInfo()
{
    return &(playersInfo);
}


player_info_t* LevelBuild::getPlayerInfo(int number)
{
    for(int i = 0; i < playersInfo.size(); i++)
    {
        if(playersInfo[i].number == number)
        {
            return &(playersInfo[i]);
        }
    }
    throw WADReadoutException("player does not exist in THINGS lump!");
}


// private

vector<player_info_t> LevelBuild::calculatePlayersInfo()
{
    vector<player_info_t> lPlayersInfo;
    player_info_t player;
    for(LevelData::Thing thing: level->getThings())
    {
        for(int x = 1; x< 5; x++)
        {
            if(thing.type == x)
            {
                player.number = x;
                player.angle = thing.angle;
                player.posX = thing.x;
                player.posY = thing.y;
                lPlayersInfo.push_back(player);
            }
        }      
    }
    
    return lPlayersInfo;
}

level_bounds_t LevelBuild::calculateLevelBounds()
{
    vector<LevelData::Vertex> vertexs = level->getVertexs();
    int lowestX = 0;
    int highestX = 0;
    int lowestY = 0;
    int highestY = 0;
    for(LevelData::Vertex vertex: vertexs)
    {
        if(vertex.x > highestX)
        {
            highestX = vertex.x;
        }
        if(vertex.x < lowestX)
        {
            lowestX = vertex.x;
        }
        if(vertex.y > highestY)
        {
            highestY = vertex.y;
        }
        if(vertex.y < lowestY)
        {
            lowestY = vertex.y;
        }
    }

    level_bounds_t bounds;
    bounds.xPos = lowestX;
    bounds.yPos = highestY;
    bounds.height = highestY - lowestY;
    bounds.width = highestX - lowestX;
    return bounds;
}


vertexs_bounds_t LevelBuild::calculateVertexsBounds()
{
    vector<LevelData::Vertex> vertexs = level->getVertexs();
    int lowestX = 0;
    int highestX = 0;
    int lowestY = 0;
    int highestY = 0;
    for(LevelData::Vertex vertex: vertexs)
    {
        if(vertex.x > highestX)
        {
            highestX = vertex.x;
        }
        if(vertex.x < lowestX)
        {
            lowestX = vertex.x;
        }
        if(vertex.y > highestY)
        {
            highestY = vertex.y;
        }
        if(vertex.y < lowestY)
        {
            lowestY = vertex.y;
        }
    }

    vertexs_bounds_t bounds;
    bounds.xPosMin= lowestX;
    bounds.yPosMax = highestY;
    bounds.xPosMax = highestX;
    bounds.yPosMin = lowestY;
    return bounds;
}