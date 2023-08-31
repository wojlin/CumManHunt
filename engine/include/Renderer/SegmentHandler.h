#ifndef SEGMENTHANDLER_H
#define SEGMENTHANDLER_H

#include "../GameData/LevelData.h"
#include "../Controller/Player.h"
#include "../Utils/Math.h"
#include "LevelBuild.h"

#include "../../engine.h"

#include <cmath>
#include <SFML/Graphics.hpp>
#include <set>
#include <vector>

class Engine;

class SegmentHandler
{
    public:

        SegmentHandler(Engine& lEngine, Player *lPlayer);

        void classifySegment(LevelData::Seg segment, int x1, int x2, float rwAngle);

        void initScreenRange();

        void clipSolidWalls(int x1, int x2);

    private:
        Engine& engine;
        Player* player;
        LevelData::LevelData* level;
        vector<LevelData::Node> nodes;
        vector<LevelData::Vertex> vertexs;
        vector<LevelData::SSector> subsectors;
        vector<LevelData::Seg> segs;
        vector<LevelData::Linedef> linedefs;
        vector<LevelData::Sidedef> sidedefs;

        std::set<int> screenRange;
};

#endif