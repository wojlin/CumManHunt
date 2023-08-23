#ifndef LEVELBUILD_H
#define LEVELBUILD_H

#include "../GameData/LevelData.h"
#include "../GameData/WadStructure.h"
#include "../Utils/Exceptions.h"


struct level_bounds_t
{
    int xPos;
    int yPos;
    int width;
    int height;
};

struct vertexs_bounds_t
{
    int xPosMin;
    int xPosMax;
    int yPosMin;
    int yPosMax;
};

struct player_info_t
{
    float number;
    float posX;
    float posY;
    float angle;
};

class LevelBuild
{
    public:

        LevelBuild(unique_ptr<LevelData::LevelData> *lLevel, WADStructure::WADStructure *lWad);

        level_bounds_t getLevelBounds();

        vertexs_bounds_t getVertexsBounds();

        vector<player_info_t>* getPlayersInfo();
        

        player_info_t* getPlayerInfo(int number);

        

    private:
        unique_ptr<LevelData::LevelData>* level;
        WADStructure::WADStructure* wad;
        level_bounds_t levelBounds;
        vertexs_bounds_t vertexsBounds;
        vector<player_info_t> playersInfo;

        vector<player_info_t> calculatePlayersInfo();

        level_bounds_t calculateLevelBounds();


        vertexs_bounds_t calculateVertexsBounds();

};


#endif