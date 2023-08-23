#ifndef RENDERER_H
#define RENDERER_H

#include "../GameData/LevelData.h"
#include "../Controller/Player.h"
#include "../Utils/Math.h"
#include "LevelBuild.h"

#include <cmath>
#include <SFML/Graphics.hpp>

namespace LevelRenderer
{

    class LevelRenderer
    {
        public:

            LevelRenderer(int lWidth, int lHeight, int lFov, unique_ptr<LevelData::LevelData> *lLevel, LevelBuild::LevelBuild *lLevelBuild, vector<Player> *lPlayers)
            {

                width = lWidth;
                height = lHeight;
                level = lLevel;
                levelBuild = lLevelBuild;
                players = lPlayers;
                fov = lFov;
                hFov = static_cast<int>((float) fov / 2.0);

                vertexsBounds = levelBuild->getVertexsBounds();
                vertexs = level->get()->getVertexs();
                lines = level->get()->getLinedefs();
                nodes = level->get()->getNodes();
                segs = level->get()->getSegs();
                
            }        

        private:
            unique_ptr<LevelData::LevelData>* level;
            LevelBuild::LevelBuild* levelBuild;
            LevelBuild::vertexs_bounds_t* vertexsBounds;
            vector<LevelData::Vertex> vertexs;
            vector<LevelData::Linedef> lines;
            vector<LevelData::Node> nodes;
            vector<LevelData::Seg> segs;
            vector<Player>* players;

            int width;
            int height;
            int fov;
            int hFov;

    };

}

#endif